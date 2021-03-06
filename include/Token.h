#ifndef M6_TOKEN_H
#define M6_TOKEN_H

#include <opcodes.h>

#define IDENTIFIER_STACK_RESERVE     0x00'00'80
#define PUNCTUATION_CHARACTERS "/?.>,<'\":]}[{=+-)(*&^%!`~"
#define WHITESPACE_CHARACTERS "\t\r\v\f "

#define NOTHING      ((token_type_t) 0)
#define ANYTHING     ((token_type_t) -1)
#define VALUE_TOKEN  ((token_type_t) (1u << 31u))
#define RANGE_TOKEN  ((token_type_t) (1u << 30u))

// Value Token Types

#define IDENTIFIER   ((token_type_t) (1u <<  1u) | VALUE_TOKEN)  // Must start with an identifier character.
// Must consist only of identifier characters and digits.
#define NUMBER       ((token_type_t) (1u <<  2u) | VALUE_TOKEN)  // Must start with a digit or a -ve sign or a dot.
// Must be a valid number subtype.
#define BOOLEAN      ((token_type_t) (1u <<  3u) | VALUE_TOKEN)  // True and false keywords.
#define KEYWORD      ((token_type_t) (1u <<  4u) | VALUE_TOKEN)  // Loops, conditions, functions, etc.
#define OPERATOR     ((token_type_t) (1u <<  5u) | VALUE_TOKEN)  // Must start with a punctuation.
// Must be a valid opcode.
// Unary and binary operator keywords belong here!
#define WHITESPACE   ((token_type_t) (1u <<  6u) | VALUE_TOKEN)  // Must start with and contain only whitespace.
// These are not tokenized, but are defined because in certain cases,
// we have to expect them.
#define EOL          ((token_type_t) (1u <<  7u) | VALUE_TOKEN)  // EOL and EOS are independent tokens,
#define EOS          ((token_type_t) (1u <<  8u) | VALUE_TOKEN)  // they only consist of one EOL or EOS character.

// Range Token Types

#define STRING       ((token_type_t) (1u <<  9u) | RANGE_TOKEN)
#define TEMPLATE     ((token_type_t) (1u << 10u) | RANGE_TOKEN)
#define REGEX        ((token_type_t) (1u << 11u) | RANGE_TOKEN)
#define COMMENT      ((token_type_t) (1u << 12u) | RANGE_TOKEN)
#define PARENTHESES  ((token_type_t) (1u << 13u) | RANGE_TOKEN)
#define BRACKETS     ((token_type_t) (1u << 14u) | RANGE_TOKEN)
#define BRACES       ((token_type_t) (1u << 15u) | RANGE_TOKEN)
#define ROOT         ((token_type_t) (1u << 16u) | RANGE_TOKEN)

/*
 * Braces/brackets/parentheses are first initalized undecided in LiteralProcessor::parse_range.
 * They get decided by the keyword balancer, which then changes the value of their opcode to something
 * that passes opcode & OP_DECIDED.
 */

// Common Subtypes
// Identifiers, booleans, whitespace, eol, eos, string, template,
// regex, and comment all have no subtypes and will always use this.
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

// Operator and keyword subtypes are present in opcodes.h:
// - OP_ASSIGNMENT to OP_START_END
// - OP_KW_BLOCK to OP_KW_OPERATOR
// This macro just discards all bits except those significant to the marking of the subtype.
#define OPCODE_TO_SUBTYPE(n) ((token_subtype_t) (n & (0x1ffu << 7u)))

// Only 32 are used for token_type_t, but we lose nothing.
// Only 8 are used for token_subtype_t in everything but operators.
// uint8_t would've been fine too, but we're too lazy to rewrite the defines in opcodes.h
// for operator subtypes.
// We rather just use them as is.
typedef uint64_t token_type_t;
typedef uint64_t token_subtype_t;


class Token {
public:
    Token (token_type_t type, token_subtype_t subtype,
           std::u16string::const_iterator begin, std::u16string::const_iterator end,
           void* value_ptr);

    [[nodiscard]] static bool is_digit (char16_t c);

    [[nodiscard]] static bool is_hexadecimal_digit (char16_t c);

    [[nodiscard]] static bool is_identifier (char16_t c);

    [[nodiscard]] static bool is_whitespace (char16_t c);

    [[nodiscard]] static bool is_punctuation (char16_t c);

    [[nodiscard]] static bool is_line_terminator (char16_t c);

    [[nodiscard]] static opcode_t kw_cstr_to_opcode (const char16_t* c);

    [[nodiscard]] static const char16_t* kw_opcode_to_cstr (opcode_t keyword_opcode);

    [[nodiscard]] bool cannot_precede_division ();

    [[nodiscard]] std::string to_string ();

    [[nodiscard]] bool is_whitespace ();

    [[nodiscard]] bool is_comment ();

    [[nodiscard]] bool is_discardable ();

    [[nodiscard]] std::u16string::const_iterator get_begin ();

    [[nodiscard]] std::u16string::const_iterator get_end ();

    std::string colorized_output ();

    std::vector<Token> token_vector;
    std::vector<std::u16string> identifier_stack;
    Token* parent;
protected:
    const token_type_t type;
    const token_subtype_t subtype;
    const std::u16string::const_iterator begin;  // Inclusive.
    const std::u16string::const_iterator end;  // Not inclusive.
    void* const value_ptr;  // Set to nullptr if not used.
};

#endif
