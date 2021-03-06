#include <LiteralProcessor.h>

// TODO: https://github.com/mtsoltan/m6/issues/1

/**
 * This function is used to let TokenTypeChecker::process_next_token know what we're expecting next.
 * For example, numbers expect whitespace or operators after them, while operators are okay
 * with anything being after them.
 * @param t
 */
void LiteralProcessor::expect (const token_type_t t) {
    // If we expect more than 4 times in a row before unexpecting, then we overflow and crash.
    this->expecting[this->expecting_iterator++] = t;
}

token_type_t LiteralProcessor::unexpect () {
    // If we unexpect when we don't have anything expected, then we underflow and crash.
    return this->expecting[--this->expecting_iterator];
}

bool LiteralProcessor::process_keyword (const opcode_t memoized) {
// If we have a memoized keyword, then just generate a token from that.
    if (memoized & OP_KEYWORD) {
        auto original_iterator = this->tokenizer_iterator;
        this->tokenizer_iterator += std::char_traits<char16_t>::length(Token::kw_opcode_to_cstr(memoized));

        if (memoized & OP_KW_BOOLEAN) {
            this->base_token->token_vector.push_back(Token(
                    BOOLEAN, UNDEFINED, original_iterator, this->tokenizer_iterator,
                    new bool(memoized == OPCODE_TRUE)));
        } else {
            this->base_token->token_vector.push_back(Token(
                    KEYWORD, UNDEFINED, original_iterator, this->tokenizer_iterator,
                    new opcode_t(memoized)));
        }

        NO_INCREMENT

        return true;
    }

    return false;  // What we have memoized from the TokenTypeChecker::next_token_is_keyword call is not a keyword!
}


/**
 * If there's a start operator at the tokenizer_iterator when this function is called, it returns
 * an iterator to the end operator of that start operator.
 * Otherwise it throws.
 * @return
 */
bool LiteralProcessor::parse_range (const std::optional<operator_t> memoized) {
    auto original_iterator = this->tokenizer_iterator;

    operator_t o = memoized.has_value() ? memoized.value() : this->process_symbol();

    const char16_t* end_operator;

    try {
        char16_t c_copy[MAX_OPERATOR_SIZE + 1];  // + 1 for \0.
        std::char_traits<char16_t>::copy(c_copy, &(*original_iterator), o.size);
        c_copy[o.size] = '\0';

        end_operator = get_begin_end_map().at(c_copy);
    } catch (const std::out_of_range& e) {
        throw ERR_INVALID_START_OPERATOR;
    }

    uint8_t end_size = std::char_traits<char16_t>::length(end_operator);

    if (o.opcode & OP_NESTABLE) {  // {, (, [
        int64_t nesting_level = 0;
        while (nesting_level >= 0) {
            // Find next start/end operators and increment decrement nesting level until it hits -1.
            bool begin_found, end_found, string_ended;
            while (
                    !(begin_found = std::char_traits<char16_t>::compare(
                            &(*++this->tokenizer_iterator), &(*original_iterator), o.size) == 0) &&
                    !(end_found = std::char_traits<char16_t>::compare(
                            &(*this->tokenizer_iterator), end_operator, end_size) == 0) &&
                    !(string_ended = this->get_char_offset() == NOT_FOUND)
                    );
            if (begin_found) {
                ++nesting_level;
            }
            if (end_found) {
                --nesting_level;
            }
            if (string_ended) {
                return false;  // Syntax error, expected closing.
            }

            if (nesting_level < 0) {
                // We found the end. Create a new token.
                // Since it's a (), [], or {}, we make it an undecided start_end nestable token.
                // The keyword balancer will decide what to make of those.
                token_type_t type =
                        o.opcode == OPCODE_PARENTHESES1 ? PARENTHESES :
                        o.opcode == OPCODE_BRACKET1 ? BRACKETS :
                        o.opcode == OPCODE_BRACES1 ? BRACES : NOTHING;

                this->base_token->token_vector.push_back(Token(
                        type, OPCODE_TO_SUBTYPE(o.opcode),
                        original_iterator, this->tokenizer_iterator += o.size, new opcode_t(o.opcode)));

                return true;
            }
        }

        // We should probably throw instead of return a syntax error?
        return false;
    } else {  // /, /*, //, `, ", '
        // Find the next end operator.
        was_escaped:  // If it was escaped, we retry finding the next one.

        while (true) {
            ++this->tokenizer_iterator;
            if (this->get_char_offset() == NOT_FOUND) {
                return false;  // Syntax error, expected closing, but code ended before closing was found.
            }
            if (Token::is_line_terminator(*this->tokenizer_iterator)) {
                if (o.opcode == OPCODE_COMMENTL) {
                    break;
                }
                if (o.opcode == OPCODE_QDOUBLE || o.opcode == OPCODE_QSINGLE || o.opcode == OPCODE_REGEX) {
                    return false;  // Syntax error, EOL encountered before string end.
                }
            }
            if (std::char_traits<char16_t>::compare(&(*this->tokenizer_iterator), end_operator, end_size) == 0) {
                break;  // We found an end operator.
            }
        }

        if (*(this->tokenizer_iterator - 1) == '\\' && o.opcode & OP_RANGE_SYM) {
            // Only symmetric opcodes can be escaped with a backslash.
            goto was_escaped;
        }

        // We found the end. Create a new token.

        // For anything but the line-comment, we add the end operator to the tokenizer_iterator.
        // The line-comment works differently, because the \n actually needs to get parsed as an EOL.
        if (o.opcode != OPCODE_COMMENTL) {
            this->tokenizer_iterator += end_size;
        }

        // If it is discovered that it is a string / regex literal (unambiguous unlike array, object,
        // function ones), the called function should process it immediately using process_x_literal in this
        // LiteralProcessor class. Be ware with the modifiers at the end of regex literals.

        // Take care of regex post modifiers in parse_regex_literal.

        // Template literals, however, will have to wait for later because they can have subscopes.
        if (o.opcode == OPCODE_COMMENT1 || o.opcode == OPCODE_COMMENTL) {
            this->base_token->token_vector.push_back(Token(
                    COMMENT, UNDEFINED, original_iterator, this->tokenizer_iterator, nullptr));
            return true;
        }

        if (o.opcode == OPCODE_QDOUBLE || o.opcode == OPCODE_QSINGLE) {
            // Keep in mind that value_ptr is not null-terminated.
            // This means that we'll have to be careful when it ends.
            this->base_token->token_vector.push_back(Token(
                    STRING, UNDEFINED, original_iterator, this->tokenizer_iterator, nullptr));
            return true;
        }

        // If it's a regex, any identifiers stuck to it will be post-modifiers.
        if (o.opcode == OPCODE_REGEX) {
            while (Token::is_identifier(*this->tokenizer_iterator)) {
                this->tokenizer_iterator++;
            }

            this->base_token->token_vector.push_back(Token(
                    REGEX, UNDEFINED, original_iterator, this->tokenizer_iterator, nullptr));
            return true;
        }

        // For now, the pre-modifiers of templates are considered separate identifiers.
        if (o.opcode == OPCODE_QTICK) {
            this->base_token->token_vector.push_back(Token(
                    TEMPLATE, UNDEFINED, original_iterator, this->tokenizer_iterator, nullptr));
            return true;
        }

        // We should probably throw and not return a syntax error?
        return false;
    }
}


bool LiteralProcessor::process_operator () {
    auto original_iterator = this->tokenizer_iterator;
    // We first try to process the symbol

    // We check the returned opcode, if it's a value token thingy, we just create the value token
    // and increment the tokenizer iterator by the operator size.
    //
    // process_symbol is a const function that does not move our tokenizer_iterator, but it does
    // provide us with an operator_t.size member that gives us a hint about how much we need to
    // increment our tokenizer_iterator.
    operator_t o = this->process_symbol();

    // We have to be very careful on OPCODE_DIV which can be the regex starter.
    // We call LiteralProcessor->next_token_is_regex to find out.
    if (this->next_token_is_regex(o)) {
        o.opcode = OPCODE_REGEX;
    }

    if (o.opcode & OP_START_END) {
        // If it's an end-only operator, throw a syntax error.
        if (!(o.opcode & OP_RANGE_START)) {
            return false;
        }

        // If it's a start operator, we need to find its end.
        return this->parse_range(o);
    } else {
        this->base_token->token_vector.push_back(Token(
                OPERATOR, OPCODE_TO_SUBTYPE(o.opcode),
                original_iterator, this->tokenizer_iterator += o.size, new opcode_t(o.opcode)));
        return true;
    }
}

bool LiteralProcessor::next_token_is_regex (const std::optional<operator_t> memoized) {
    operator_t o = memoized.has_value() ? memoized.value() : this->process_symbol();

    if (o.opcode != OPCODE_DIV) {  // If it doesn't start with the division symbol, it can't be regex.
        return false;
    }

    // We look at the last token in our token vector:
    auto last_token = this->base_token->token_vector.end();  // std::vector<Token>::iterator

    while ((*--last_token).is_discardable());  // Comments and whitespace should not affect our lookbehind.

    while (true) {
        try {
            if ((*last_token).cannot_precede_division()) return true;
        } catch (error_t e) {
            last_token--;
            continue;
        }

        break;
    }

    // The token before could precede division, so we'll assume this is division and not regex.
    return false;
}

/**
 * Once we know that we've encountered an identifier, we can process it using this method.
 * This method will change the position of this->tokenizer_iterator to after the identifier,
 * allowing us to continue processing.
 * @return
 */
bool LiteralProcessor::process_identifier () {
    auto original_iterator = this->tokenizer_iterator;

    // An identifier consists only of identifier characters and digit characters.
    for (; this->get_char_offset() != NOT_FOUND; ++this->tokenizer_iterator) {
        if (!Token::is_identifier(*this->tokenizer_iterator) && !Token::is_digit(*this->tokenizer_iterator)) {
            break;
        }
    }

    std::u16string identifier(original_iterator, this->tokenizer_iterator);

    // This is the base_token of this LiteralProcessor. In the loop below, it will keep bubbling up
    // through parents.
    auto curent_token = this->base_token;
    std::vector<std::u16string>::iterator position;
    while (true) {
        position = std::find(
                curent_token->identifier_stack.begin(), curent_token->identifier_stack.end(), identifier);
        if (position == curent_token->identifier_stack.end()) {
            // If we don't find the identifier:
            if (curent_token->parent != nullptr) {
                // If a parent exists, we bubble up.
                curent_token = curent_token->parent;
            } else {
                // If we bubbled up all the way to the end and didn't find that specific identifier,
                // we just push to the bottom-most scope (the base token).
                this->base_token->identifier_stack.push_back(identifier);
                break;
            }
        } else {
            // If we find the identifier somewhere, we just break since position is now at the value we want it.
            break;
        }


    }

    // Create a token with the identifier index in the identifier stack.
    this->base_token->token_vector.push_back(Token(
            IDENTIFIER, UNDEFINED, original_iterator, this->tokenizer_iterator,
            (void*)(&*position)));

    // We don't need to increment in this function because no matter what way the for ends (the break or normal end),
    // we have reached something that isn't part of this identifier.
    NO_INCREMENT

    return true;  // Anything can show up suddenly mid-identifier, so identifiers never return false.
}

/**
 * Once we know that we've encountered a number, we can process it using this method.
 * This method will change the position of this->tokenizer_iterator to after the identifier,
 * allowing us to continue processing.
 * @return
 */
bool LiteralProcessor::process_number_literal () {
    auto original_iterator = this->tokenizer_iterator;

    token_subtype_t subtype = UNDEFINED;

    int64_t accumulator = 0;
    int64_t decimal_accumulator = 0;  // For a special case in which a number starts with zero yet is decimal.
    double accumulator_f = 0.0;
    int8_t sign = 1;
    bool strict_oct = false;

    for (; this->get_char_offset() != NOT_FOUND; ++this->tokenizer_iterator) {
        // If it starts with a negative sign, we set the sign to -1.
        // We will multiply by this sign at the very end.
        if (*this->tokenizer_iterator == '-' && subtype == UNDEFINED) {
            sign = -1;
            continue;
        }

        // If it starts with zero, we know it could be a special integer.
        if (*this->tokenizer_iterator == '0' && subtype == UNDEFINED) {
            subtype |= INT_SPEC;
            continue;
        }

        // If we know it could be special and we find an x, we know it's hexadecimal.
        if ((*this->tokenizer_iterator == 'x' || *this->tokenizer_iterator == 'X') && subtype == INT_SPEC) {
            subtype |= INT_HEX;
            continue;
        }

        // If we know it could be special and we find a b, we know it's binary.
        if ((*this->tokenizer_iterator == 'b' || *this->tokenizer_iterator == 'B') && subtype == INT_SPEC) {
            subtype |= INT_BIN;
            continue;
        }

        // If we know it could be special and we find an o, we know it's octal.
        if ((*this->tokenizer_iterator == 'o' || *this->tokenizer_iterator == 'O') && subtype == INT_SPEC) {
            subtype |= INT_OCT;
            strict_oct = true;
            continue;
        }

        // If we know it's not a decimal and we find a digit, we know it's octal.
        if (Token::is_digit(*this->tokenizer_iterator) && subtype == INT_SPEC) {
            subtype = INT_OCT;  // We do not continue here, as this is an actual digit of the number.
        }

        // We assume it's decimal since it didn't start with zero, until proven otherwise
        // by a dot or an exp.
        //
        // Note that the subtype change here also makes it so that numbers starting with a dot will
        // pass the condition (  *this->tokenizer_iterator == '.' && subtype == INT_DEC  ).
        if (subtype == UNDEFINED) {
            subtype = INT_DEC;
        }

        // We accumulate each digit to the value of the token.
        if (Token::is_digit(*this->tokenizer_iterator)) {
            accumulator *= subtype;
            decimal_accumulator *= INT_DEC;
            uint8_t temp = *this->tokenizer_iterator - '0';
            accumulator += temp;
            decimal_accumulator += temp;

            if (temp > 1 && subtype == INT_BIN) {
                return false;
            }

            // Numbers including numbers higher than 7 can't be octal, and get the decimal treatment.
            if (temp > 7 && subtype == INT_OCT) {
                if (strict_oct) {
                    return false;
                }
                accumulator = decimal_accumulator;
                subtype = INT_DEC;
            }

            continue;
        }

        // If we find a hexadecimal digit in a hexadecimal number, we convert and accumulate.
        if (Token::is_hexadecimal_digit(*this->tokenizer_iterator) && subtype == INT_HEX) {
            accumulator *= subtype;
            uint8_t temp = *this->tokenizer_iterator - 'a' + 10;
            accumulator += temp;
        }

        // If we find a dot or an exp in an octal number, we auto assume it's decimal from now on.
        if ((*this->tokenizer_iterator == '.' || *this->tokenizer_iterator == 'e') && subtype == INT_OCT) {
            accumulator = decimal_accumulator;
            subtype = INT_DEC;
        }

        // If it's a decimal so far, and we find a dot, we convert our accumulator to a float accumulator,
        // then reset it to prepare it for holding the fractional component.
        if (*this->tokenizer_iterator == '.' && subtype == INT_DEC) {
            subtype = FLOAT_N;
            accumulator_f = (double) accumulator;
            accumulator = 0;
            continue;
        }

        // If we discover it's an exponential float, we end the mantissa here and expect a number next (the exponent).
        if (*this->tokenizer_iterator == 'e' && (subtype == INT_DEC || subtype == FLOAT_N)) {
            subtype = subtype == INT_DEC ? FLOAT_E : FLOAT_NE;
            this->expect(NUMBER);

            // We increment here because we don't want to be parsing the 'e' again as an identifier.
            ++this->tokenizer_iterator;

            break;
        }

        // If we find a non-identifier, non-dot, we have reached the end of the number and we break.
        if (!Token::is_identifier(*this->tokenizer_iterator) && *this->tokenizer_iterator != '.') {
            // We do not increment here, as this thing we just found isn't part of the number,
            // but rather a part of the next token.
            NO_INCREMENT

            break;
        }

        // Cases like finding a second dot, or an x or b mid number, or some other garbage all lead here.
        return false;
    }

    // If we're on float we add accumulator as the decimal fraction of accumulator_f.
    if (subtype & FLOAT_N) {
        double temp = accumulator;
        while (temp > 1) temp /= 10;
        accumulator_f += temp;
        accumulator_f *= sign;
        this->base_token->token_vector.push_back(Token(
                NUMBER, subtype, original_iterator, this->tokenizer_iterator,
                new double(accumulator_f)));
    } else {
        accumulator *= sign;
        this->base_token->token_vector.push_back(Token(
                NUMBER, subtype, original_iterator, this->tokenizer_iterator,
                new int64_t(accumulator)));
    }

    return true;
}
