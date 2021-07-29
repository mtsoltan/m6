#ifndef M6_OPCODES_H
#define M6_OPCODES_H

// TODO: https://github.com/mtsoltan/m6/issues/2
#include <map>
#include <cinttypes>

#define OP_KEYWORD         ((opcode_t) (1u << 15u))
#define OP_KEYWORD_END     ((opcode_t) (KEYWORD_END_ - 1))
#define OP_KEYWORD_SIZE    (sizeof("synchronized") / sizeof(char))

#define OP_UNARY_L         ((opcode_t) (1u << 14u))
#define OP_UNARY_R         ((opcode_t) (1u << 13u))
#define OP_UNARY           ((opcode_t) (OP_UNARY_L | OP_UNARY_R))  // What is not unary is binary.

#define OP_ASSIGNMENT      ((opcode_t) (7u << 10u))
#define OP_COMPARISON      ((opcode_t) (6u << 10u))
#define OP_ARITHMETIC      ((opcode_t) (5u << 10u))
#define OP_BITWISE         ((opcode_t) (4u << 10u))
#define OP_LOGICAL         ((opcode_t) (3u << 10u))
#define OP_START_END       ((opcode_t) (1u << 9u))

typedef uint16_t opcode_t;

enum opcode_enum_t : opcode_t {
    OPCODE_NOOP,

    OPCODE_QMARK,
    OPCODE_COLON,  // Used in trinary operators, cases, labels, and object property declaration.

    OPCODE_COMMA,
    OPCODE_DQMARK,
    OPCODE_ARROW,

    OPCODE_DOT,
    OPCODE_TRIPLEDOT,
    OPCODE_DOTQMARK,

    OPCODE_A = OP_ASSIGNMENT,
    OPCODE_AADD,
    OPCODE_ASUB,
    OPCODE_AMUL,
    OPCODE_ADIV,
    OPCODE_AREM,
    OPCODE_APWR,
    OPCODE_ASHL,
    OPCODE_ASHR,
    OPCODE_ASHRU,
    OPCODE_AANDB,
    OPCODE_AXORB,
    OPCODE_AORB,
    OPCODE_AANDL,
    OPCODE_AORL,
    OPCODE_ANULL,  // ??=

    OPCODE_EQ = OP_COMPARISON,
    OPCODE_NE,
    OPCODE_EQE,  // Strict
    OPCODE_NEE,
    OPCODE_GT,
    OPCODE_GTE,
    OPCODE_LT,
    OPCODE_LTE,

    OPCODE_ADD = OP_ARITHMETIC,
    OPCODE_SUB,
    OPCODE_MUL,
    OPCODE_DIV,  // Divide is also the regex starter / ender.
    OPCODE_REM,
    OPCODE_PWR,

    OPCODE_INC = OP_UNARY | OP_ARITHMETIC,
    OPCODE_DEC,

    OPCODE_USUB = OP_UNARY_L | OP_ARITHMETIC,
    OPCODE_UADD,

    OPCODE_ANDB = OP_BITWISE,
    OPCODE_ORB,
    OPCODE_XORB,
    OPCODE_SHL,
    OPCODE_SHR,
    OPCODE_SHRU,  // >>>, the zero fill right shift

    OPCODE_NOTB = OP_UNARY_L | OP_BITWISE,

    OPCODE_NOTL = OP_UNARY_L | OP_LOGICAL,

    OPCODE_ANDL = OP_LOGICAL,
    OPCODE_ORL,

    OPCODE_QDOUBLE = OP_START_END,
    OPCODE_QSINGLE,
    OPCODE_QTICK,

    OPCODE_PARENTHESES1,
    OPCODE_PARENTHESES2,
    OPCODE_BRACKET1,
    OPCODE_BRACKET2,
    OPCODE_CURLY1,
    OPCODE_CURLY2,
    OPCODE_COMMENT1,
    OPCODE_COMMENT2,
    OPCODE_COMMENTL,

    // TODO: https://github.com/mtsoltan/m6/issues/2#issuecomment-887923183
    OPCODE_BREAK = OP_KEYWORD,  // Permanently reserved.
    OPCODE_CASE,
    OPCODE_CATCH,
    OPCODE_CLASS,
    OPCODE_CONST,
    OPCODE_CONTINUE,
    OPCODE_DEBUGGER,
    OPCODE_DEFAULT,
    OPCODE_DELETE,
    OPCODE_DO,
    OPCODE_ELSE,
    OPCODE_EXPORT,
    OPCODE_EXTENDS,
    OPCODE_FINALLY,
    OPCODE_FOR,
    OPCODE_FUNCTION,
    OPCODE_IF,
    OPCODE_IMPORT,
    OPCODE_IN,
    OPCODE_INSTANCEOF,
    OPCODE_NEW,
    OPCODE_RETURN,
    OPCODE_SUPER,
    OPCODE_SWITCH,
    OPCODE_THIS,
    OPCODE_THROW,
    OPCODE_TRY,
    OPCODE_TYPEOF,
    OPCODE_VAR,
    OPCODE_VOID,
    OPCODE_WHILE,
    OPCODE_WITH,
    OPCODE_YIELD,

    OPCODE_ENUM,  // Future permanently reserved.

    OPCODE_IMPLEMENTS,  // Future strict mode reserved.
    OPCODE_INTERFACE,
    OPCODE_LET,
    OPCODE_PACKAGE,
    OPCODE_PRIVATE,
    OPCODE_PROTECTED,
    OPCODE_PUBLIC,
    OPCODE_STATIC,

    OPCODE_AWAIT,  // Future reserved in module code.

    OPCODE_ABSTRACT,  // Reserved by older standards.
    OPCODE_BOOLEAN,
    OPCODE_BYTE,
    OPCODE_CHAR,
    OPCODE_DOUBLE,
    OPCODE_FINAL,
    OPCODE_FLOAT,
    OPCODE_GOTO,
    OPCODE_INT,
    OPCODE_LONG,
    OPCODE_NATIVE,
    OPCODE_SHORT,
    OPCODE_SYNCHRONIZED,
    OPCODE_THROWS,
    OPCODE_TRANSIENT,
    OPCODE_VOLATILE,

    OPCODE_NULL,  // Used by the language so cannot be used as identifiers.
    OPCODE_TRUE,
    OPCODE_FALSE,
    KEYWORD_END_,

};

const char KEYWORDS[OP_KEYWORD_END - OP_KEYWORD + 1][OP_KEYWORD_SIZE] = {
        "break",
        "case",
        "catch",
        "class",
        "const",
        "continue",
        "debugger",
        "default",
        "delete",
        "do",
        "else",
        "export",
        "extends",
        "finally",
        "for",
        "function",
        "if",
        "import",
        "in",
        "instanceof",
        "new",
        "return",
        "super",
        "switch",
        "this",
        "throw",
        "try",
        "typeof",
        "var",
        "void",
        "while",
        "with",
        "yield",

        "enum",

        "implements",
        "interface",
        "let",
        "package",
        "private",
        "protected",
        "public",
        "static",

        "await",

        "abstract",
        "boolean",
        "byte",
        "char",
        "double",
        "final",
        "float",
        "goto",
        "int",
        "long",
        "native",
        "short",
        "synchronized",
        "throws",
        "transient",
        "volatile",

        "null",
        "true",
        "false",
};

#endif