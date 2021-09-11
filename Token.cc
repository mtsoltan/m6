#include <Token.h>
#include <colors.h>  // Specified here because everything else that includes tokens should not need colors.

bool Token::is_digit (const char16_t c) {
    return c >= '0' && c <= '9';
}

bool Token::is_hexadecimal_digit (const char16_t c) {
    return (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

bool Token::is_identifier (const char16_t c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '$';
}

bool Token::is_whitespace (const char16_t c) {
    for (int i = 0; i < sizeof(WHITESPACE_CHARACTERS) / sizeof(char) - 1; ++i) {
        if (c == WHITESPACE_CHARACTERS[i]) {
            return true;
        }
    }
    return false;
}

bool Token::is_line_terminator (const char16_t c) {
    return c == '\r' || c == '\n';
}

opcode_t Token::kw_cstr_to_opcode (const char16_t* const c) {
    uint8_t end = 0;
    char16_t c_copy[OP_KEYWORD_SIZE];

    for (; end < OP_KEYWORD_SIZE; ++end) {
        if (!Token::is_identifier(c[end])) {  // Keywords can only include identifier characters.
            break;
        }
    }

    std::char_traits<char16_t>::copy(c_copy, c, end);
    c_copy[end] = '\0';

    try {
        return get_kw_cstr_opcode_map().at(c_copy);
    } catch (const std::out_of_range& e) {
        return OPCODE_NOOP;
    }
}

const char16_t* Token::kw_opcode_to_cstr (const opcode_t keyword_opcode) {
    return get_kw_opcode_cstr_map().at(keyword_opcode);
}

bool Token::is_punctuation (const char16_t c) {
    for (int i = 0; i < sizeof(PUNCTUATION_CHARACTERS) / sizeof(char) - 1; ++i) {
        if (c == PUNCTUATION_CHARACTERS[i]) {
            return true;
        }
    }
    return false;
}

Token::Token (token_type_t type, token_subtype_t subtype, std::u16string::const_iterator begin,
              std::u16string::const_iterator end, void* value_ptr)
        : type(type), subtype(subtype), begin(begin), end(end), value_ptr(value_ptr) {
    this->identifier_stack.reserve(IDENTIFIER_STACK_RESERVE);
}

bool Token::cannot_precede_division () {
    if (this->type == EOL) {
        // Ambiguous, check before it.
        throw 0;
    }

    if (this->type == COMMENT) {
        // Ambiguous, check before it.
        throw 0;
    }

    if (this->type == EOS) {
        return true;
    }

    // At the time this function runs, range operators are all still very basic tokens,
    // and their internals have not yet been parsed. They all have an opcode of their start,
    // and span to their end.
    //
    // Ambiguous, needs to get decided first. We'll assume that the most common usage case of
    // parentheses gets followed by division, while the most common usage case of braces gets
    // followed by regex.
    //
    // TODO: https://github.com/mtsoltan/m6/issues/18
    if (this->type == BRACES) {
        return true;
    }
    if (this->type == PARENTHESES) {
        return false;
    }

    if (this->type == OPERATOR) {
        opcode_t opcode = *((opcode_t*) this->value_ptr);

        if (opcode & OP_UNARY) {  // ++ and --
            // Ambiguous. Postfix ones can precede division, prefix ones cannot.
            throw 0;
        }

        return opcode & OP_ASSIGNMENT ||
               opcode & OP_COMPARISON ||
               opcode & OP_ARITHMETIC ||
               opcode & OP_BITWISE ||
               opcode & OP_LOGICAL ||
               opcode == OPCODE_QMARK ||
               opcode == OPCODE_COLON ||
               opcode == OPCODE_COMMA ||
               opcode == OPCODE_DOT ||
               opcode == OPCODE_TRIPLEDOT;
    }

    if (this->type == KEYWORD) {
        opcode_t opcode = *((opcode_t*) this->value_ptr);
        return opcode & OP_KW_OPERATOR ||
               opcode & OP_KW_DECLARE ||  // All of those cannot precede either division or regex!
               opcode & OP_KW_BLOCK ||  // Some of those cannot precede eiter division or regex!
               opcode & OP_KW_BLOCK_EXT;  // Some of those cannot precede eiter division or regex!
    }

    // Identifiers, numbers, strings, booleans, brackets, templates, regex (whitespace do not exist).
    return false;  // all have to preceed division.
}

std::string Token::to_string () {
    using namespace srilakshmikanthanp;
    static const auto
            R = ansi::str(ansi::fg_red),
            G = ansi::str(ansi::fg_green),
            B = ansi::str(ansi::fg_blue),
            C = ansi::str(ansi::fg_cyan),
            M = ansi::str(ansi::fg_magenta),
            Y = ansi::str(ansi::fg_yellow),
            K = ansi::str(ansi::reset);

    std::string J;

    switch (this->type) {
        case IDENTIFIER:
            J = R; break;
        case NUMBER:
        case BOOLEAN:
            J = G; break;
        case OPERATOR:
            J = B; break;
        case KEYWORD:
            J = C; break;
        case STRING:
        case REGEX:
        case TEMPLATE:
            J = M; break;
        case COMMENT:
            J = Y; break;
        default:
            J = K; break;
    }

    return J + toUTF8(std::u16string(this->begin, this->end)) + K;
}

bool Token::is_whitespace () {
    return this->type == WHITESPACE;
}

bool Token::is_comment () {
    return this->type == COMMENT;
}

bool Token::is_discardable () {
    return this->is_whitespace() || this->is_comment();
}

std::u16string::const_iterator Token::get_begin () {
    return this->begin;
}

std::u16string::const_iterator Token::get_end () {
    return this->end;
}


std::string Token::colorized_output () {
    std::string rv;

    for (auto& token: this->token_vector) {
        rv += token.to_string();
    }

    return rv;
}
