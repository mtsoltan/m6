#ifndef M6_TOKEN_H
#define M6_TOKEN_H

#include <cstring>

#include <toplev.h>

#define PUNCTUATION_CHARACTERS "/?.>,<'\":]}[{=+-)(*&^%!`~"
#define WHITESPACE_CHARACTERS "\t\r\v\f "

#define NOTHING      ((token_type_t) 0)
#define ANYTHING     ((token_type_t) -1)
#define VALUE_TOKEN  ((token_type_t) (1u << 31u))
#define RANGE_TOKEN  ((token_type_t) (1u << 30u))

// Value Token Types

#define IDENTIFIER   ((token_type_t) (1u << 1u) | VALUE_TOKEN)  // Must start with an identifier character.
                                                                // Must consist only of identifier characters and digits.
#define NUMBER       ((token_type_t) (1u << 2u) | VALUE_TOKEN)  // Must start with a digit or a negative sign or a dot.
                                                                // Must be a valid number subtype.
#define BOOLEAN      ((token_type_t) (1u << 3u) | VALUE_TOKEN)  // True and false keywords.
#define KEYWORD      ((token_type_t) (1u << 4u) | VALUE_TOKEN)  // Loops, conditions, functions, etc.
#define OPERATOR     ((token_type_t) (1u << 5u) | VALUE_TOKEN)  // Must start with a punctuation.
                                                                // Must be a valid opcode.
                                                                // Unary and binary operator keywords belong here!
#define WHITESPACE   ((token_type_t) (1u << 6u) | VALUE_TOKEN)  // Must start with whitespace and contain only whitespace.
                                                                // These are not tokenized, but are defined because in certain cases,
                                                                // we have to expect them.
#define EOL          ((token_type_t) (1u << 7u) | VALUE_TOKEN)  // EOL and EOS are independent tokens, they only consist of one EOL
#define EOS          ((token_type_t) (1u << 8u) | VALUE_TOKEN)  // or EOS character.

// Range Token Types

#define STRING       ((token_type_t) (1u << 9u) | RANGE_TOKEN)
#define TEMPLATE     ((token_type_t) (1u << 10u) | RANGE_TOKEN)
#define REGEX        ((token_type_t) (1u << 11u) | RANGE_TOKEN)
#define COMMENT      ((token_type_t) (1u << 12u) | RANGE_TOKEN)
#define PARENTHESES  ((token_type_t) (1u << 13u) | RANGE_TOKEN)
#define BRACKETS     ((token_type_t) (1u << 14u) | RANGE_TOKEN)
#define CURLIES      ((token_type_t) (1u << 15u) | RANGE_TOKEN)


// Common Subtypes
#define UNDEFINED    ((token_subtype_t)  0)

// Number Subtypes
#define INT_SPEC     ((token_subtype_t)  1)  // 0?_____
#define INT_BIN      ((token_subtype_t)  2)  // 0b_____
#define INT_OCT      ((token_subtype_t)  8)  // 0______
#define INT_DEC      ((token_subtype_t) 10)  // _______
#define INT_HEX      ((token_subtype_t) 16)  // 0x_____
#define FLOAT_E      ((token_subtype_t) 64)  // ____e__
#define FLOAT_N      ((token_subtype_t) 68)  // ___.___
#define FLOAT_NE     ((token_subtype_t) 69)  // __.__e_

// Operator Subtypes
// TODO: https://github.com/mtsoltan/m6/issues/9

typedef uint32_t token_type_t;
typedef uint8_t token_subtype_t;


// TODO: https://github.com/mtsoltan/m6/issues/8
class Token {
public:

    static bool is_digit(char c);
    static bool is_hexadecimal_digit(char c);
    static bool is_identifier(char c);
    static bool is_whitespace(char c);
    static bool is_punctuation(char c);
    static bool is_line_terminator(char c);
    static opcode_t cstr_to_keyword(const char c[OP_KEYWORD_SIZE]);
protected:
    token_type_t type;
    token_subtype_t subtype;
};


class ValueToken : public Token {
public:
    ValueToken(token_type_t type, void* value_ptr, token_subtype_t subtype = UNDEFINED);
protected:
    token_type_t type;
    token_subtype_t subtype;
    void* value_ptr;
};


class RangeToken {
public:
    RangeToken(token_type_t type, std::string::iterator start, std::string::iterator end, token_subtype_t subtype = UNDEFINED);
protected:
    token_type_t type;
    token_subtype_t subtype;
    std::string::iterator start;  // Inclusive.
    std::string::iterator end;  // Not inclusive.
};


#endif
