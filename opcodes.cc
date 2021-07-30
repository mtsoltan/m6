#include <opcodes.h>

const opcode_cstr_map& get_op_opcode_cstr_map (const uint8_t operator_size = 0) {
    static const opcode_cstr_map op_opcode_cstr_map_1 {
            {OPCODE_QMARK,        "?"},
            {OPCODE_COLON,        ":"},  // Used in trinary operators, cases, labels,
            {OPCODE_COMMA,        ","},  // and object property declaration.
            {OPCODE_DOT,          "."},
            {OPCODE_A,            "="},
            {OPCODE_GT,           ">"},
            {OPCODE_LT,           "<"},
            {OPCODE_ADD,          "+"},  // OPCODE_USUB
            {OPCODE_SUB,          "-"},  // OPCODE_UADD
            {OPCODE_MUL,          "*"},
            {OPCODE_DIV,          "/"},  // OPCODE_REGEX1, OPCODE_REGEX2
            {OPCODE_REM,          "%"},
            {OPCODE_ANDB,         "&"},
            {OPCODE_ORB,          "|"},
            {OPCODE_XORB,         "^"},
            {OPCODE_NOTB,         "~"},
            {OPCODE_NOTL,         "!"},
            {OPCODE_QDOUBLE,      "\""},
            {OPCODE_QSINGLE,      "'"},
            {OPCODE_QTICK,        "`"},
            {OPCODE_PARENTHESES1, "("},  // Used in keyword blocks, function literals, grouping, and
            {OPCODE_PARENTHESES2, ")"},  // function calls.
            {OPCODE_BRACKET1,     "["},  // Used in array access, array literals, and array appending.
            {OPCODE_BRACKET2,     "]"},
            {OPCODE_BRACES1,      "{"},  // Used in keyword blocks, label blocks, and object literals.
            {OPCODE_BRACES2,      "}"},
    };
    static const opcode_cstr_map op_opcode_cstr_map_2 {
            {OPCODE_NULLC,    "??"},
            {OPCODE_ARROW,    "=>"},
            {OPCODE_DOTQMARK, ".?"},
            {OPCODE_AADD,     "+="},
            {OPCODE_ASUB,     "-="},
            {OPCODE_AMUL,     "*="},
            {OPCODE_ADIV,     "/="},
            {OPCODE_AREM,     "%="},
            {OPCODE_AANDB,    "&="},
            {OPCODE_AXORB,    "^="},
            {OPCODE_AORB,     "|="},
            {OPCODE_EQ,       "=="},
            {OPCODE_NE,       "!="},
            {OPCODE_GTE,      ">="},
            {OPCODE_LTE,      "<="},
            {OPCODE_PWR,      "**"},
            {OPCODE_INC,      "++"},
            {OPCODE_DEC,      "--"},
            {OPCODE_SHL,      "<<"},
            {OPCODE_SHR,      ">>"},
            {OPCODE_ANDL,     "&&"},
            {OPCODE_ORL,      "||"},
            {OPCODE_COMMENT1, "/*"},
            {OPCODE_COMMENT2, "*/"},
            {OPCODE_COMMENTL, "//"},
    };
    static const opcode_cstr_map op_opcode_cstr_map_3 {
            {OPCODE_APWR,      "**="},
            {OPCODE_ASHL,      "<<="},
            {OPCODE_ASHR,      ">>="},
            {OPCODE_TRIPLEDOT, "..."},
            {OPCODE_AANDL,     "&&="},
            {OPCODE_AORL,      "||="},
            {OPCODE_ANULLC,    "?\?="},
            {OPCODE_EQE,       "==="},
            {OPCODE_NEE,       "!=="},
            {OPCODE_SHRU,      ">>>"},
    };
    static const opcode_cstr_map op_opcode_cstr_map_4 {
            {OPCODE_ASHRU, ">>>="},
    };

    static opcode_cstr_map op_opcode_cstr_map_0_builder {};
    op_opcode_cstr_map_0_builder.merge(*const_cast<opcode_cstr_map*>(&op_opcode_cstr_map_4));
    op_opcode_cstr_map_0_builder.merge(*const_cast<opcode_cstr_map*>(&op_opcode_cstr_map_3));
    op_opcode_cstr_map_0_builder.merge(*const_cast<opcode_cstr_map*>(&op_opcode_cstr_map_2));
    op_opcode_cstr_map_0_builder.merge(*const_cast<opcode_cstr_map*>(&op_opcode_cstr_map_1));

    static const opcode_cstr_map op_opcode_cstr_map_0 =
            *const_cast<const opcode_cstr_map*>(&op_opcode_cstr_map_0_builder);

    switch (operator_size) {
        case 4:
            return op_opcode_cstr_map_4;
        case 3:
            return op_opcode_cstr_map_3;
        case 2:
            return op_opcode_cstr_map_2;
        case 1:
            return op_opcode_cstr_map_1;
        case 0:
            return op_opcode_cstr_map_0;
        default:
            throw ERR_OPERATOR_INVALID_SIZE;
    }
}

const opcode_cstr_map& get_kw_opcode_cstr_map () {
    static const opcode_cstr_map kw_opcode_cstr_map {
            {OPCODE_BREAK,        "break"},
            {OPCODE_CASE,         "case"},
            {OPCODE_CATCH,        "catch"},
            {OPCODE_CLASS,        "class"},
            {OPCODE_CONST,        "const"},
            {OPCODE_CONTINUE,     "continue"},
            {OPCODE_DEBUGGER,     "debugger"},
            {OPCODE_DEFAULT,      "default"},
            {OPCODE_DELETE,       "delete"},
            {OPCODE_DO,           "do"},
            {OPCODE_ELSE,         "else"},
            {OPCODE_EXPORT,       "export"},
            {OPCODE_EXTENDS,      "extends"},
            {OPCODE_FINALLY,      "finally"},
            {OPCODE_FOR,          "for"},
            {OPCODE_FUNCTION,     "function"},
            {OPCODE_IF,           "if"},
            {OPCODE_IMPORT,       "import"},
            {OPCODE_IN,           "in"},
            {OPCODE_OF,           "of"},
            {OPCODE_INSTANCEOF,   "instanceof"},
            {OPCODE_NEW,          "new"},
            {OPCODE_RETURN,       "return"},
            {OPCODE_SUPER,        "super"},
            {OPCODE_SWITCH,       "switch"},
            {OPCODE_THIS,         "this"},
            {OPCODE_THROW,        "throw"},
            {OPCODE_TRY,          "try"},
            {OPCODE_TYPEOF,       "typeof"},
            {OPCODE_VAR,          "var"},
            {OPCODE_VOID,         "void"},
            {OPCODE_WHILE,        "while"},
            {OPCODE_WITH,         "with"},
            {OPCODE_YIELD,        "yield"},

            {OPCODE_ENUM,         "enum"},

            {OPCODE_IMPLEMENTS,   "implements"},
            {OPCODE_INTERFACE,    "interface"},
            {OPCODE_LET,          "let"},
            {OPCODE_PACKAGE,      "package"},
            {OPCODE_PRIVATE,      "private"},
            {OPCODE_PROTECTED,    "protected"},
            {OPCODE_PUBLIC,       "public"},
            {OPCODE_STATIC,       "static"},

            {OPCODE_AWAIT,        "await"},

            {OPCODE_ABSTRACT,     "abstract"},
            {OPCODE_BOOLEAN,      "boolean"},
            {OPCODE_BYTE,         "byte"},
            {OPCODE_CHAR,         "char"},
            {OPCODE_DOUBLE,       "double"},
            {OPCODE_FINAL,        "final"},
            {OPCODE_FLOAT,        "float"},
            {OPCODE_GOTO,         "goto"},
            {OPCODE_INT,          "int"},
            {OPCODE_LONG,         "long"},
            {OPCODE_NATIVE,       "native"},
            {OPCODE_SHORT,        "short"},
            {OPCODE_SYNCHRONIZED, "synchronized"},
            {OPCODE_THROWS,       "throws"},
            {OPCODE_TRANSIENT,    "transient"},
            {OPCODE_VOLATILE,     "volatile"},

            {OPCODE_NULL,         "null"},
            {OPCODE_TRUE,         "true"},
            {OPCODE_FALSE,        "false"},
    };
    return kw_opcode_cstr_map;
}

const cstr_opcode_map& get_kw_cstr_opcode_map () {
    static const cstr_opcode_map kw_cstr_opcode_map {
            {"break",        OPCODE_BREAK},
            {"case",         OPCODE_CASE},
            {"catch",        OPCODE_CATCH},
            {"class",        OPCODE_CLASS},
            {"const",        OPCODE_CONST},
            {"continue",     OPCODE_CONTINUE},
            {"debugger",     OPCODE_DEBUGGER},
            {"default",      OPCODE_DEFAULT},
            {"delete",       OPCODE_DELETE},
            {"do",           OPCODE_DO},
            {"else",         OPCODE_ELSE},
            {"export",       OPCODE_EXPORT},
            {"extends",      OPCODE_EXTENDS},
            {"finally",      OPCODE_FINALLY},
            {"for",          OPCODE_FOR},
            {"function",     OPCODE_FUNCTION},
            {"if",           OPCODE_IF},
            {"import",       OPCODE_IMPORT},
            {"in",           OPCODE_IN},
            {"of",           OPCODE_OF},
            {"instanceof",   OPCODE_INSTANCEOF},
            {"new",          OPCODE_NEW},
            {"return",       OPCODE_RETURN},
            {"super",        OPCODE_SUPER},
            {"switch",       OPCODE_SWITCH},
            {"this",         OPCODE_THIS},
            {"throw",        OPCODE_THROW},
            {"try",          OPCODE_TRY},
            {"typeof",       OPCODE_TYPEOF},
            {"var",          OPCODE_VAR},
            {"void",         OPCODE_VOID},
            {"while",        OPCODE_WHILE},
            {"with",         OPCODE_WITH},
            {"yield",        OPCODE_YIELD},

            {"enum",         OPCODE_ENUM},

            {"implements",   OPCODE_IMPLEMENTS},
            {"interface",    OPCODE_INTERFACE},
            {"let",          OPCODE_LET},
            {"package",      OPCODE_PACKAGE},
            {"private",      OPCODE_PRIVATE},
            {"protected",    OPCODE_PROTECTED},
            {"public",       OPCODE_PUBLIC},
            {"static",       OPCODE_STATIC},

            {"await",        OPCODE_AWAIT},

            {"abstract",     OPCODE_ABSTRACT},
            {"boolean",      OPCODE_BOOLEAN},
            {"byte",         OPCODE_BYTE},
            {"char",         OPCODE_CHAR},
            {"double",       OPCODE_DOUBLE},
            {"final",        OPCODE_FINAL},
            {"float",        OPCODE_FLOAT},
            {"goto",         OPCODE_GOTO},
            {"int",          OPCODE_INT},
            {"long",         OPCODE_LONG},
            {"native",       OPCODE_NATIVE},
            {"short",        OPCODE_SHORT},
            {"synchronized", OPCODE_SYNCHRONIZED},
            {"throws",       OPCODE_THROWS},
            {"transient",    OPCODE_TRANSIENT},
            {"volatile",     OPCODE_VOLATILE},

            {"null",         OPCODE_NULL},
            {"true",         OPCODE_TRUE},
            {"false",        OPCODE_FALSE},
    };
    return kw_cstr_opcode_map;
}
