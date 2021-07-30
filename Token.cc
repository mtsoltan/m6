#include <Token.h>

bool Token::is_digit (const char c) {
    return c >= '0' && c <= '9';
}

bool Token::is_hexadecimal_digit (const char c) {
    return (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

bool Token::is_identifier (const char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '$';
}

bool Token::is_whitespace (const char c) {
    for (int i = 0; i < sizeof(WHITESPACE_CHARACTERS) / sizeof(char) - 1; ++i) {
        if (c == WHITESPACE_CHARACTERS[i]) {
            return true;
        }
    }
    return false;
}

bool Token::is_line_terminator (const char c) {
    return c == '\r' || c == '\n';
}

opcode_t Token::cstr_to_opcode (const char* const c) {
    uint8_t end = 0;
    char c_copy[OP_KEYWORD_SIZE];

    for (; end < OP_KEYWORD_SIZE; ++end) {
        if (Token::is_whitespace(c[end])) {
            break;
        }
    }

    strncpy(c_copy, c, end);
    c_copy[end] = '\0';

    try {
        return get_kw_cstr_opcode_map().at(c_copy);
    } catch (const std::out_of_range& e) {
        return OPCODE_NOOP;
    }
}

const char* Token::opcode_to_cstr (const opcode_t keyword_opcode) {
    return get_kw_opcode_cstr_map().at(keyword_opcode);
}

bool Token::is_punctuation (const char c) {
    for (int i = 0; i < sizeof(PUNCTUATION_CHARACTERS) / sizeof(char) - 1; ++i) {
        if (c == PUNCTUATION_CHARACTERS[i]) {
            return true;
        }
    }
    return false;
}

Token::Token (token_type_t type, token_subtype_t subtype, std::string::const_iterator start,
              std::string::const_iterator end) : type(type), subtype(subtype), start(start), end(end) {}

ValueToken::ValueToken (token_type_t type, token_subtype_t subtype, std::string::const_iterator start,
                        std::string::const_iterator end, void* value_ptr) : Token(type, subtype, start, end),
                                                                            value_ptr(value_ptr) {}
