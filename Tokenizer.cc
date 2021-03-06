#include <Tokenizer.h>


Tokenizer::Tokenizer (int log_handler (const char*, ...)) : LiteralProcessor(log_handler) {
    // Overload the constructor.
    this->expect(ANYTHING);
}

/**
 * Attempts to tokenize a given two iterators for the start and end of a string or string segment.
 *
 * @param file_contents
 * @return
 */
Token
Tokenizer::tokenize (const std::u16string::const_iterator& begin, const std::u16string::const_iterator& end) {
    // We need to make a reference to what the previous base token and token iterator were.
    // This is so that recursive calls of this function can work properly.
    // This is similar to pushing to stack in the figurative sense.
    auto old_tokenizer_iterator = this->tokenizer_iterator;
    auto old_base_token = this->base_token;

    this->tokenizer_iterator = begin;  // Copy assign begin and end here.

    auto rv = Token(ROOT, UNDEFINED, begin, end, nullptr);
    rv.token_vector.reserve(TOKEN_VECTOR_RESERVE);

    this->base_token = &rv;

    // Attempt to process the next token forever till process_next_token returns false.
    // It will return false when done or when it encounters an error.
    while (this->process_next_token());

    if (this->get_char_offset() != NOT_FOUND) {
        throw ERR_TOKENIZING_SYNTAX_ERROR;
    }

    // We return them as they were. You can consider this an action similar to popping a stack.
    this->tokenizer_iterator = old_tokenizer_iterator;
    this->base_token = old_base_token;

    return rv;
}

/**
 * Attempts to tokenize a given string, usually a file contents.
 * @param file_contents
 * @return
 */
Token Tokenizer::tokenize (const std::u16string& str) {
    return this->tokenize(str.begin(), str.end());
}

/**
 * Simply reads a file and then attempts to tokenize it.
 * @param file_name
 * @return
 */
Token Tokenizer::tokenize (const char* file_name) {
    std::ifstream file(file_name, std::ios::binary | std::ios::in);

    if (file.fail()) {
        throw ERR_IFSTREAM_FAILED;
    }

    if (file.is_open()) {
        file.seekg(0, std::ios::beg);
        auto tmp_u8_str = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        fromUTF8(tmp_u8_str, this->content);

        file.close();
        return this->tokenize(this->content);
    }

    throw ERR_IFSTREAM_FAILED;
}

/**
 * Repeatedly discovers whether the next token is an identifier, number, or operator, then
 * calls the correct process function on it.
 * Will return false if the process function returns false (an intra-token syntax error).
 *
 * If we are expecting a specific token type, and it discovers a token of a different type,
 * it just returns false (an inter-token syntax error).
 * @return
 */
bool Tokenizer::process_next_token () {
    auto original_iterator = this->tokenizer_iterator;
    // Uses this->tokenizer_iterator to either process_identifier, process_number_literal, or process_symbol.
    bool rv = false;

    // First, we check what we're expecting.
    token_type_t expected_type = this->unexpect();

    if (this->get_char_offset() == NOT_FOUND) {
        return false;
    }

    // If it's whitespace, we just skip past it and do nothing.
    if (Token::is_whitespace(*this->tokenizer_iterator) && (expected_type & WHITESPACE)) {
        while (Token::is_whitespace(*(++this->tokenizer_iterator)));
        this->base_token->token_vector.emplace_back(
                WHITESPACE, UNDEFINED, original_iterator, this->tokenizer_iterator, nullptr);
        rv = true;
        goto expect;
    }

    // If it's an EOL or EOS, we just skip past it and empalce it.
    if (Token::is_line_terminator(*this->tokenizer_iterator) && (expected_type & EOL)) {
        this->base_token->token_vector.emplace_back(
                EOL, UNDEFINED, original_iterator, ++this->tokenizer_iterator, nullptr);
        rv = true;
        goto expect;
    }

    if (*this->tokenizer_iterator == ';' && (expected_type & EOS)) {
        this->base_token->token_vector.emplace_back(
                EOS, UNDEFINED, original_iterator, ++this->tokenizer_iterator, nullptr);
        rv = true;
        goto expect;
    }

    // The logic for figuring whether it's an operator or a number is a little complex,
    // and is abstracted to a function of its own.
    if (this->next_token_is_number()) {
        rv = this->process_number_literal();
        goto expect;
    }

    // Operators have to be processed before identifiers so that "var" and "let" do not end up being recognized
    // as identifiers.
    if (Token::is_punctuation(*this->tokenizer_iterator)) {
        rv = this->process_operator();
        goto expect;
    }

    // Operators have to be processed before identifiers so that "var" and "let" do not end up being recognized
    // as identifiers.
    if (opcode_t memoized = this->next_token_is_keyword()) {
        rv = this->process_keyword(memoized);
        goto expect;
    }


    // If it's an identifier, we process it.
    if (Token::is_identifier(*this->tokenizer_iterator) && (expected_type & IDENTIFIER)) {
        rv = this->process_identifier();
        goto expect;
    }

    // If execution reaches this point, we have run into an invalid sequence.

    expect:
    // If we have nothing queued to uninspect on the next call, we'll just expect ANYTHING.
    // This prevents us from underflowing on the negative side.
    if (this->expecting_iterator == 0) {
        this->expect(ANYTHING);
    }

    // Returns false once done, or once any subprocess returns false, or if an error has been encountered.
    return rv;
}
