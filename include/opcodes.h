#ifndef M6_OPCODES_H
#define M6_OPCODES_H

#define FIRST_KEYWORD      OPCODE_BREAK
#define LAST_KEYWORD       OPCODE_FALSE
#define BIGGEST_KEYWORD    (sizeof("synchronized") / sizeof(char))

typedef unsigned char opcode_t;

// TODO: Turn those into bitmask with bits for:
//  2 bits - Start/end operators
//  2 bits - number of operands - 1  (care for function literals when implementing for(a;b;c){d} etc.
//  1 bits - Is keyword?
enum opcode_enum_t : opcode_t {
    OPCODE_NOOP,

    OPCODE_A,
    OPCODE_AADD,
    OPCODE_ASUB,
    OPCODE_AMUL,
    OPCODE_ADIV,
    OPCODE_AREM,
    OPCODE_APWR,
    OPCODE_AANDB,
    OPCODE_AANDL,
    OPCODE_AORB,
    OPCODE_AORL,
    OPCODE_ANOTB,
    OPCODE_ANOTL,
    OPCODE_AXORB,
    OPCODE_AXORL,

    OPCODE_ARROW,

    OPCODE_LT,
    OPCODE_GT,
    OPCODE_LTE,
    OPCODE_GTE,
    OPCODE_EQ,
    OPCODE_EQE,
    OPCODE_NE,
    OPCODE_NEE,

    OPCODE_DOT,
    OPCODE_TRIPLEDOT,
    OPCODE_DOTQMARK,
    OPCODE_COMMA,
    OPCODE_QMARK,
    OPCODE_DQMARK,
    OPCODE_COLON,  // Used in trinary operators, cases, and labels.

    OPCODE_QDOUBLE,
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

    OPCODE_DIV,  // Divide is also the regex starter / ender.
    OPCODE_MUL,
    OPCODE_PWR,
    OPCODE_SUB,
    OPCODE_ADD,
    OPCODE_UADD,
    OPCODE_REM,

    OPCODE_ANDB,
    OPCODE_ANDL,
    OPCODE_ORB,
    OPCODE_ORL,
    OPCODE_NOTB,
    OPCODE_NOTL,
    OPCODE_XORB,
    OPCODE_XORL,
    OPCODE_SHL,
    OPCODE_SHR,
    OPCODE_SHLI,
    OPCODE_SHRI,

    OPCODE_INC,
    OPCODE_DEC,

    OPCODE_BREAK,  // Permanently reserved.
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

};

const char KEYWORDS[LAST_KEYWORD - FIRST_KEYWORD + 1][BIGGEST_KEYWORD] = {
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