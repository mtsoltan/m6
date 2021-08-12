#ifndef M6_OPCODES_H
#define M6_OPCODES_H

// TODO: https://github.com/mtsoltan/m6/issues/15
#include <map>
#include <cinttypes>
#include <cstring>

#include <errors.h>

#define MAX_OPERATOR_SIZE 4

#define OP_KEYWORD         ((opcode_t) (1u << 15u))
#define OP_KEYWORD_SIZE    ((uint8_t)(sizeof("synchronized") / sizeof(char)))


#define OP_UNARY_L         ((opcode_t) (1u << 14u))
#define OP_UNARY_R         ((opcode_t) (1u << 13u))
#define OP_UNARY           ((opcode_t) (OP_UNARY_L | OP_UNARY_R))  // What is not unary is binary.

#define OP_RANGE_END       ((opcode_t) (1u << 14u))
#define OP_RANGE_START     ((opcode_t) (1u << 13u))
#define OP_RANGE_SYM       ((opcode_t) (OP_RANGE_END | OP_RANGE_START))

#define OP_ASSIGNMENT      ((opcode_t) (7u << 10u))
#define OP_COMPARISON      ((opcode_t) (6u << 10u))
#define OP_ARITHMETIC      ((opcode_t) (5u << 10u))
#define OP_BITWISE         ((opcode_t) (4u << 10u))
#define OP_LOGICAL         ((opcode_t) (3u << 10u))
#define OP_START_END       ((opcode_t) (1u <<  9u))
#define OP_NESTABLE        ((opcode_t) (1u <<  8u))
#define OP_DECIDED         ((opcode_t) (1u <<  7u))


#define OP_KW_BINARY       ((opcode_t) (3u << 13u))
#define OP_KW_UNARY        ((opcode_t) (2u << 13u))  // 0-nary operators should always be in statements of their own.
#define OP_KW_0NARY        ((opcode_t) (1u << 13u))  // They include the debugger, and control flow keywords.

#define OP_KW_BLOCK        ((opcode_t) (7u << 10u))
#define OP_KW_BLOCK_EXT    ((opcode_t) (6u << 10u))
#define OP_KW_UNUSED       ((opcode_t) (5u << 10u))
#define OP_KW_BOOLEAN      ((opcode_t) (4u << 10u))
#define OP_KW_MISCLITERAL  ((opcode_t) (3u << 10u))
#define OP_KW_DECLARE      ((opcode_t) (2u << 10u))
#define OP_KW_OPERATOR     ((opcode_t) (1u << 10u))

typedef uint64_t opcode_t;  // We only use 15 bits, but for signedness.

enum opcode_enum_t : opcode_t {
    OPCODE_NOOP,

    // OPERATORS

    OPCODE_QMARK,
    OPCODE_COLON,

    OPCODE_COMMA,
    OPCODE_NULLC,
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
    OPCODE_ANULLC,

    OPCODE_EQ = OP_COMPARISON,
    OPCODE_NE,
    OPCODE_EQE,
    OPCODE_NEE,
    OPCODE_GT,
    OPCODE_GTE,
    OPCODE_LT,
    OPCODE_LTE,

    OPCODE_ADD = OP_ARITHMETIC,
    OPCODE_SUB,
    OPCODE_MUL,
    OPCODE_DIV,
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
    OPCODE_SHRU,

    OPCODE_NOTB = OP_UNARY_L | OP_BITWISE,

    OPCODE_NOTL = OP_UNARY_L | OP_LOGICAL,

    OPCODE_ANDL = OP_LOGICAL,
    OPCODE_ORL,

    OPCODE_QDOUBLE = OP_START_END | OP_RANGE_SYM,
    OPCODE_QSINGLE,
    OPCODE_QTICK,
    OPCODE_REGEX,

    OPCODE_PARENTHESES1 = OP_START_END | OP_RANGE_START | OP_NESTABLE,
    OPCODE_BRACKET1,
    OPCODE_BRACES1,

    OPCODE_FNCALL = OP_START_END | OP_RANGE_START | OP_NESTABLE | OP_DECIDED,
    OPCODE_GROUPING,
    OPCODE_FNPARAM,
    OPCODE_ARRAY,
    OPCODE_ACCESS,
    OPCODE_APPEND,
    OPCODE_OBJECT,
    OPCODE_SCOPE,

    OPCODE_COMMENT1 = OP_START_END | OP_RANGE_START,
    OPCODE_COMMENTL,

    OPCODE_PARENTHESES2 = OP_START_END | OP_RANGE_END,
    OPCODE_BRACKET2,
    OPCODE_BRACES2,
    OPCODE_COMMENT2,

    // KEYWORDS

    // Used by the language so cannot be used as identifiers.
    OPCODE_THIS = OP_KEYWORD | OP_KW_MISCLITERAL,
    OPCODE_SUPER,
    OPCODE_NULL,
    OPCODE_TRUE = OP_KEYWORD | OP_KW_BOOLEAN,
    OPCODE_FALSE,

    // Permanently reserved.
    OPCODE_CONST = OP_KEYWORD | OP_KW_DECLARE | OP_KW_UNARY,
    OPCODE_VAR,
    OPCODE_LET,  // <-- Reserved in SM only.
    OPCODE_STATIC,

    OPCODE_IN = OP_KEYWORD | OP_KW_OPERATOR | OP_KW_BINARY,
    OPCODE_OF,  // <-- This one is not reserved, but has special usage inside a for..of loop.
    OPCODE_INSTANCEOF,

    OPCODE_DELETE = OP_KEYWORD | OP_KW_OPERATOR | OP_KW_UNARY,
    OPCODE_NEW,
    OPCODE_EXPORT,
    OPCODE_IMPORT,
    OPCODE_TYPEOF,
    OPCODE_YIELD,
    OPCODE_AWAIT,  // <-- Future reserved in module code.
    OPCODE_VOID,

    OPCODE_BREAK = OP_KEYWORD | OP_KW_OPERATOR | OP_KW_0NARY,
    OPCODE_CONTINUE,  // Break, continue, return, throw can be both unary or 0-nary.
    OPCODE_RETURN,
    OPCODE_THROW,
    OPCODE_DEBUGGER,  // Debugger is 0-nary only.

    OPCODE_TRY = OP_KEYWORD | OP_KW_BLOCK,
    OPCODE_IF,
    OPCODE_SWITCH,
    OPCODE_DO,
    OPCODE_FOR,
    OPCODE_WHILE,
    OPCODE_FUNCTION,

    OPCODE_CLASS,
    OPCODE_EXTENDS,

    OPCODE_CASE,
    OPCODE_DEFAULT,

    OPCODE_FINALLY = OP_KEYWORD | OP_KW_BLOCK_EXT,
    OPCODE_CATCH,
    OPCODE_ELSE,

    OPCODE_ENUM = OP_KEYWORD | OP_KW_UNUSED,  // <-- Future permanently reserved.

    OPCODE_WITH,

    // Future strict mode reserved.
    OPCODE_IMPLEMENTS,
    OPCODE_INTERFACE,
    OPCODE_PACKAGE,
    OPCODE_PRIVATE,
    OPCODE_PROTECTED,
    OPCODE_PUBLIC,

    // Reserved by older standards.
    OPCODE_ABSTRACT,
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
};

struct char_cmp {
    bool operator() (const char* a, const char* b) const {
        return strcmp(a, b) < 0;
    }
};

typedef std::map<opcode_t, const char*> opcode_cstr_map;
typedef std::map<const char*, opcode_t, char_cmp> cstr_opcode_map;
typedef std::map<const char*, const char*, char_cmp> cstr_cstr_map;

const opcode_cstr_map& get_op_opcode_cstr_map (uint8_t operator_size = 0);

const cstr_opcode_map& get_op_cstr_opcode_map (uint8_t operator_size = 0);

const opcode_cstr_map& get_kw_opcode_cstr_map ();

const cstr_opcode_map& get_kw_cstr_opcode_map ();

const cstr_cstr_map& get_start_end_map ();

#endif
