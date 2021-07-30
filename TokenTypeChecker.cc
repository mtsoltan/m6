#include <TokenTypeChecker.h>

TokenTypeChecker::TokenTypeChecker (int log_handler (const char*, ...)) {
    this->log_handler = log_handler;
    this->token_vector.reserve(TOKEN_VECTOR_RESERVE);
}

/**
 * Once we know that we've encountered a symbol, we can process it using this method.
 * This method will change the position of this->tokenizer_iterator to after the identifier,
 * allowing us to continue processing.
 * @return
 */
bool TokenTypeChecker::process_symbol () {
    // Reaching this point means that we have a punctuation symbol.
    // First, we need to recognize possible operators based on the first punctuation mark,
    // then compare the second, third, etc. till we settle on a specific operator.
    // TODO: https://github.com/mtsoltan/m6/issues/7
    std::string::const_iterator original_iterator = this->tokenizer_iterator;

    // We check how many symbols are in a row.
    // If more than 4, we set the maximum per single operator to 4.
    // At the end of this loop, the token iterator will be on the character after the possible operator.
    uint8_t ecx = MAX_OPERATOR_SIZE;
    while (Token::is_punctuation(*(++this->tokenizer_iterator)) && --ecx);

    // We iterate operator size starting at the maximum, decrementing till we reach 1.
    // We compare operator size from our current tokenizer iterator to everything in the opcode map of that size.
    bool found = false;
    // while (!found) {
    //     uint8_t operator_size = this->tokenizer_iterator - original_iterator;
    //     if (strncmp(*(&original_iterator), ))
    // }

    ++this->tokenizer_iterator;
    return true;
}

int64_t TokenTypeChecker::get_char_offset () const {
    if (this->tokenizer_iterator == this->tokenizer_iterator_end || *this->tokenizer_iterator == '\0') {
        return NOT_FOUND;  // This whole function needs to be signed, because NOT_FOUND is negative.
    }

    return this->tokenizer_iterator - this->tokenizer_iterator_begin;
}

bool TokenTypeChecker::next_token_is_number () const {
    std::string::const_iterator temp = this->tokenizer_iterator;

    // If it starts with a negative sign, then we ignore it, since it might very well be a number.
    if (*temp == '-') {
        ++temp;
    }

    // Falling through from the negative sign (order is important),
    // if it starts with a dot, then we ignore it, since it might very well be a number.
    if (*temp == '.') {
        ++temp;
    }

    // Falling back through from the optional negative sign and dot,
    // anything that starts with a digit is automatically a number.
    return Token::is_digit(*temp);
}

opcode_t TokenTypeChecker::next_token_is_keyword () const {
    return Token::cstr_to_opcode(&(*this->tokenizer_iterator));
}
