#ifndef M6_TOKEN_H
#define M6_TOKEN_H

#include <cstring>

#include <toplev.h>

#define PUNCTUATION_CHARACTERS "/?.>,<'\":]}[{=+-)(*&^%!`~"
#define WHITESPACE_CHARACTERS "\t\r\v\f "

// Token Types
#define NOTHING      (token_type_t) 0
#define ANYTHING     (token_type_t) 0xff'ff'ff'ff
// Must start with an identifier character.
// Must consist only of identifier characters and digits.
#define IDENTIFIER   (token_type_t) 1
// Must start with a digit or a negative sign or a dot.
// Must be a valid number subtype.
#define NUMBER       (token_type_t) 2
// Must start with a punctuation.
// Must be a valid opcode.
#define OPERATOR     (token_type_t) 4
// Must start with whitespace and contain only whitespace.
// These are not tokenized, but are defined because in certain cases, we have to expect them.
#define WHITESPACE   (token_type_t) 8
// EOL and EOS are independent tokens, they only consist of one EOL or EOS character.
#define EOL          (token_type_t) 16
#define EOS          (token_type_t) 32
// We never expect strings, but we construct them when we run into a string operator.
#define STRING       (token_type_t) 64
#define COMMENT      (token_type_t) 128

// Common Subtypes
#define UNDEFINED    (token_subtype_t) 0

// Number Subtypes
#define INT_SPEC     (token_subtype_t) 1   // 0?_____
#define INT_BIN      (token_subtype_t) 2   // 0b_____
#define INT_OCT      (token_subtype_t) 8   // 0______
#define INT_DEC      (token_subtype_t) 10  // _______
#define INT_HEX      (token_subtype_t) 16  // 0x_____
#define FLOAT_E      (token_subtype_t) 64  // ____e__
#define FLOAT_N      (token_subtype_t) 68  // ___.___
#define FLOAT_NE     (token_subtype_t) 69  // __.__e_

// Operator Subtypes
// TODO: Check how I want to go about those.

typedef unsigned int token_type_t;
typedef unsigned char token_subtype_t;

class Token {
public:
    Token(token_type_t type, long value, token_subtype_t subtype = UNDEFINED);

    static bool is_digit(char c);
    static bool is_hexadecimal_digit(char c);
    static bool is_identifier(char c);
    static bool is_whitespace(char c);
    static bool is_punctuation(char c);
    static bool is_line_terminator(char c);
    static opcode_t cstr_to_keyword(const char c[BIGGEST_KEYWORD]);
protected:
    token_type_t type;
    token_subtype_t subtype;
    long value;
};


#endif
