#include <LiteralProcessor.h>

// TODO: https://github.com/mtsoltan/m6/issues/1

LiteralProcessor::LiteralProcessor (int log_handler (const char*, ...)) : TokenTypeChecker(log_handler) {
    // Overload the constructor.
    this->identifier_stack.reserve(IDENTIFIER_STACK_RESERVE);
}

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
        // TODO: https://github.com/mtsoltan/m6/issues/6
        std::string::const_iterator original_iterator = this->tokenizer_iterator;
        this->tokenizer_iterator += strlen(Token::opcode_to_cstr(memoized));
        this->token_vector.push_back(
                new ValueToken(KEYWORD, UNDEFINED, original_iterator, this->tokenizer_iterator,
                               new opcode_t(memoized)));
        ++this->tokenizer_iterator;
        return true;
    }

    return false;  // What we have memoized from the TokenTypeChecker::next_token_is_keyword call is not a keyword!
}

bool LiteralProcessor::process_operator () {
    // We first try to process the symbol

    // We check the returned opcode, if it's a value token thingy, we just create the value token
    // and increment the tokenizer iterator by the operator size.
    //
    // process_symbol is a const function that does not move our tokenizer_iterator, but it does
    // provide us with an operator_t.size member that gives us a hint about how much we need to
    // increment our tokenizer_iterator.
    operator_t o = this->process_symbol();

    if (o.opcode & OP_START_END) {
        // If it's an end-only operator, throw a syntax error.
        if (!(o.opcode & OP_RANGE_START)) {
            return false;
        }

        // If it's a start operator, we need to find its end.
        std::string::const_iterator it = this->find_end_of_start_operator();

        // If it is discovered that it is a string / regex literal (unambiguous unlike array, object,
        // function ones), the called function should process it immediately using process_x_literal in this
        // LiteralProcessor class. Be ware with the modifiers at the end of regex literals.
        ;  // TODO: https://github.com/mtsoltan/m6/issues/7

        // If it is however a (), [], or {}, a separate function is called to find its end.
        // The separate function will increment the tokenizer iterator for us to the end, taking care of nesting.
        // We then make a () / [] / {} range token, without specifying what type of () / [], {} it is.
        // Template literals will have to wait for later as well because they can have subscopes.
        // The keyword balancer will decide what to make of those.
        ;  // TODO: https://github.com/mtsoltan/m6/issues/7

        return true;
    } else {
        // We have to be very careful on OPCODE_DIV which can be the regex starter.
        ;  // TODO: https://stackoverflow.com/questions/5519596/when-parsing-javascript-what-determines-the-meaning-of-a-slash

        this->token_vector.push_back(
                new ValueToken(OPERATOR, OPCODE_TO_SUBTYPE(o.opcode), this->tokenizer_iterator,
                               this->tokenizer_iterator += o.size, new opcode_t(o.opcode)));
        return true;
    }
}

bool LiteralProcessor::next_token_is_regex (const operator_t* const memoized) {
    operator_t o;
    if (memoized == nullptr) {
        o = this->process_symbol();
    } else {
        o = *memoized;
    }

    if (o.opcode != OPCODE_DIV) {  // If it doesn't start with the division symbol, it can't be regex.
        return false;
    }

    // We look at the last token in our token vector:
    std::vector<ValueToken*>::iterator last_token = this->token_vector.end() - 1; // NOLINT(hicpp-use-auto,modernize-use-auto)

    while (true) {
        try {
            if ((*last_token)->cannot_precede_division()) return true;
        } catch(error_t e) {
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
    std::string::const_iterator original_iterator = this->tokenizer_iterator;

    // An identifier consists only of identifier characters and digit characters.
    for (; this->get_char_offset() != NOT_FOUND; ++this->tokenizer_iterator) {
        if (!Token::is_identifier(*this->tokenizer_iterator) && !Token::is_digit(*this->tokenizer_iterator)) {
            break;
        }
    }

    std::string identifier = std::string(original_iterator, this->tokenizer_iterator);
    auto position = std::find(this->identifier_stack.begin(), this->identifier_stack.end(), identifier);

    uint64_t value;  // An index in a vector, unsigned.

    if (position == this->identifier_stack.end()) {
        value = this->identifier_stack.size();
        this->identifier_stack.push_back(identifier);
    } else {
        value = position - this->identifier_stack.begin();
    }

    // Create a token with the identifier index in the identifier stack.
    this->token_vector.push_back(
            new ValueToken(IDENTIFIER, UNDEFINED, original_iterator, this->tokenizer_iterator,
                           new uint64_t(value)));

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
    std::string::const_iterator original_iterator = this->tokenizer_iterator;

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
        this->token_vector.push_back(
                new ValueToken(NUMBER, subtype, original_iterator, this->tokenizer_iterator,
                               new double(accumulator_f)));
    } else {
        accumulator *= sign;
        this->token_vector.push_back(
                new ValueToken(NUMBER, subtype, original_iterator, this->tokenizer_iterator,
                               new int64_t(accumulator)));
    }

    return true;
}
