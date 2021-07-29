#include <Token.h>

bool Token::is_digit (char c) {
    return c >= '0' && c <= '9';
}

bool Token::is_hexadecimal_digit (char c) {
    return (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

bool Token::is_identifier (char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '$';
}

bool Token::is_whitespace (char c) {
    for (int i = 0; i < sizeof(WHITESPACE_CHARACTERS) / sizeof(char); ++i) {
        if (c == WHITESPACE_CHARACTERS[i]) {
            return true;
        }
    }
    return false;
}

bool Token::is_line_terminator (char c) {
    return c == '\r' || c == '\n';
}

opcode_t Token::cstr_to_opcode (const char c[OP_KEYWORD_SIZE]) {
    uint8_t end = 0;
    for (; end < OP_KEYWORD_SIZE; ++end) {
        if (Token::is_whitespace(c[end])) {
            break;
        }
    }
    for (auto & it : *get_kw_map()) {
        if (strncmp(it.second, c, end) == 0) {
            return it.first;
        }
    }

    return OPCODE_NOOP;
}

const char* Token::opcode_to_cstr (const opcode_t keyword_opcode) {
    return get_kw_map()->at(keyword_opcode);
}

bool Token::is_punctuation (char c) {
    for (int i = 0; i < sizeof(PUNCTUATION_CHARACTERS) / sizeof(char); ++i) {
        if (c == PUNCTUATION_CHARACTERS[i]) {
            return true;
        }
    }
    return false;
}


ValueToken::ValueToken(token_type_t type, void* value_ptr,
                       token_subtype_t subtype) : Token(), type(type), value_ptr(value_ptr), subtype(subtype) {}

RangeToken::RangeToken(token_type_t type, std::string::iterator start, std::string::iterator end,
                       token_subtype_t subtype) : Token(), type(type), start(start), end(end), subtype(subtype) {}
