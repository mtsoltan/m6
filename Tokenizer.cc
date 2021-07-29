#include <Tokenizer.h>


Tokenizer::Tokenizer (int log_handler (const char*, ...)) : LiteralProcessor(log_handler) {
    // Overload the constructor.
    this->expect(ANYTHING);
}

/**
 * This function is used to let TokenTypeChecker::process_next_token know what we're expecting next.
 * For example, numbers expect whitespace or operators after them, while operators are okay
 * with anything being after them.
 * @param t
 */
TokenTypeChecker* TokenTypeChecker::expect (token_type_t t) {
    // If we expect more than 4 times in a row before unexpecting, then we overflow and crash.
    this->expecting[this->expecting_iterator++] = t;

    return this;
}

token_type_t TokenTypeChecker::unexpect () {
    // If we unexpect when we don't have anything expected, then we underflow and crash.
    return this->expecting[--this->expecting_iterator];
}

/**
 * Attempts to tokenize a given string, usually a file contents.
 * @param file_contents
 * @return
 */
std::vector<Token*>& Tokenizer::tokenize (std::string str) {
    this->tokenizer_iterator = str.begin();
    this->tokenizer_string = str;

    // Attempt to process the next token forever till process_next_token returns false.
    // It will return false when done or when it encounters an error.
    while (this->process_next_token());

    if (this->get_char_offset() != NOT_FOUND) {
        throw ERR_TOKENIZING_SYNTAX_ERROR;
    }

    return this->token_vector;
}

/**
 * Simply reads a file and then attempts to tokenize it.
 * @param file_name
 * @return
 */
std::vector<Token*>& Tokenizer::tokenize (const char* file_name) {
    std::ifstream file(file_name, std::ios::binary | std::ios::in);

    if (file.fail()) {
        throw ERR_IFSTREAM_FAILED;
    }

    if (file.is_open()) {
        file.seekg(0, std::ios::beg);
        std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        file.close();
        this->log_handler("We now have the contents:\n%s\n", str.c_str());
        return this->tokenize(str);
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
    // Uses this->tokenizer_iterator to either process_identifier, process_number_literal, or process_symbol.
    bool rv;

    // First, we check what we're expecting.
    token_type_t expected_type = this->unexpect();

    if (this->get_char_offset() == NOT_FOUND) {
        return false;
    }

    // If it's whitespace, we just skip past it and do nothing.
    if (Token::is_whitespace(*this->tokenizer_iterator) && (expected_type & WHITESPACE)) {
        ++this->tokenizer_iterator;
        rv = true;
        goto expect;
    }

    // If it's an EOL or EOS, we just skip past it and empalce it.
    if (Token::is_line_terminator(*this->tokenizer_iterator) && (expected_type & EOL)) {
        this->token_vector.push_back(
                new ValueToken(EOL, UNDEFINED, this->tokenizer_iterator, ++this->tokenizer_iterator, nullptr));
        rv = true;
        goto expect;
    }
    if (*this->tokenizer_iterator == ';' && (expected_type & EOS)) {
        this->token_vector.push_back(
                new ValueToken(EOS, UNDEFINED, this->tokenizer_iterator, ++this->tokenizer_iterator, nullptr));
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
        rv = this->process_symbol();
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

    // We have run into an invalid sequence.
    rv = false;

    expect:
    // If we have nothing queued to uninspect on the next call, we'll just expect ANYTHING.
    // This prevents us from underflowing on the negative side.
    if (this->expecting_iterator == 0) {
        this->expect(ANYTHING);
    }

    // Returns false once done, or once any subprocess returns false, or if an error has been encountered.
    return rv;
}

