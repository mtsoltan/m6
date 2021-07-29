#include <LiteralProcessor.h>

// TODO: https://github.com/mtsoltan/m6/issues/1

bool LiteralProcessor::process_keyword (const opcode_t memoized) {
// If we have a memoized keyword, then just generate a token from that.
    if (memoized & OP_KEYWORD) {
        // TODO: https://github.com/mtsoltan/m6/issues/6
        this->token_vector.push_back(new ValueToken(KEYWORD, new opcode_t(memoized)));
        this->tokenizer_iterator += strlen(Token::opcode_to_cstr(memoized)) + 1;
        return true;
    }

    return false;  // What we have memoized from the TokenTypeChecker::next_token_is_keyword call is not a keyword!
}

/**
 * Once we know that we've encountered an identifier, we can process it using this method.
 * This method will change the position of this->tokenizer_iterator to after the identifier,
 * allowing us to continue processing.
 * @return
 */
bool LiteralProcessor::process_identifier () {
    std::string::iterator original_iterator = this->tokenizer_iterator;

    // An identifier consists only of identifier characters and digit characters.
    for (;this->get_char_offset() != NOT_FOUND; ++this->tokenizer_iterator) {
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
    this->token_vector.push_back(new ValueToken(IDENTIFIER, new uint64_t(value)));

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
    auto subtype = (token_subtype_t) UNDEFINED;

    int64_t accumulator = 0;
    int64_t decimal_accumulator = 0;  // For a special case in which a number starts with zero yet is decimal.
    double accumulator_f = 0.0;
    int8_t sign = 1;
    bool strict_oct = false;

    for (;this->get_char_offset() != NOT_FOUND; ++this->tokenizer_iterator) {
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
            auto temp = *this->tokenizer_iterator - '0';
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
            auto temp = *this->tokenizer_iterator - 'a' + 10;
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

        // If we find a non-identifier, we have reached the end of the number and we break.
        if (!Token::is_identifier(*this->tokenizer_iterator)) {
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
        auto temp = (double) accumulator;
        while (temp > 1) temp /= 10;
        accumulator_f += temp;
        accumulator_f *= sign;
        this->token_vector.push_back(new ValueToken(NUMBER, new double(accumulator_f), subtype));
    } else {
        accumulator *= sign;
        this->token_vector.push_back(new ValueToken(NUMBER, new int64_t(accumulator), subtype));
    }

    return true;
}
