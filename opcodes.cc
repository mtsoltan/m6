#include <opcodes.h>

const cstr_cstr_map& get_begin_end_map () {
    static const cstr_cstr_map begin_end_map {
            {u"{", u"}"},
            {u"(", u")"},
            {u"[", u"]"},
            {u"/", u"/"},
            {u"/*", u"*/"},
            {u"`", u"`"},
            {u"\"", u"\""},
            {u"'", u"'"},
            {u"//", u"\n"},  // Comments should end on carriage return too, but we'll ignore that for now.
    };

    return begin_end_map;
}

const opcode_cstr_map& get_op_opcode_cstr_map (const uint8_t operator_size) {
    static const opcode_cstr_map op_opcode_cstr_map_1 {
            {OPCODE_QMARK,        u"?"},
            {OPCODE_COLON,        u":"},  // Used in trinary operators, cases, labels,
            {OPCODE_COMMA,        u","},  // and object property declaration.
            {OPCODE_DOT,          u"."},
            {OPCODE_A,            u"="},
            {OPCODE_GT,           u">"},
            {OPCODE_LT,           u"<"},
            {OPCODE_ADD,          u"+"},  // OPCODE_USUB
            {OPCODE_SUB,          u"-"},  // OPCODE_UADD
            {OPCODE_MUL,          u"*"},
            {OPCODE_DIV,          u"/"},  // OPCODE_REGEX
            {OPCODE_REM,          u"%"},
            {OPCODE_ANDB,         u"&"},
            {OPCODE_ORB,          u"|"},
            {OPCODE_XORB,         u"^"},
            {OPCODE_NOTB,         u"~"},
            {OPCODE_NOTL,         u"!"},
            {OPCODE_QDOUBLE,      u"\""},
            {OPCODE_QSINGLE,      u"'"},
            {OPCODE_QTICK,        u"`"},
            {OPCODE_PARENTHESES1, u"("},  // Used in keyword blocks, function literals, grouping, and
            {OPCODE_PARENTHESES2, u")"},  // function calls.
            {OPCODE_BRACKET1,     u"["},  // Used in array access, array literals, and array appending.
            {OPCODE_BRACKET2,     u"]"},
            {OPCODE_BRACES1,      u"{"},  // Used in keyword blocks, label blocks, and object literals.
            {OPCODE_BRACES2,      u"}"},
    };
    static const opcode_cstr_map op_opcode_cstr_map_2 {
            {OPCODE_NULLC,    u"??"},
            {OPCODE_ARROW,    u"=>"},
            {OPCODE_DOTQMARK, u".?"},
            {OPCODE_AADD,     u"+="},
            {OPCODE_ASUB,     u"-="},
            {OPCODE_AMUL,     u"*="},
            {OPCODE_ADIV,     u"/="},
            {OPCODE_AREM,     u"%="},
            {OPCODE_AANDB,    u"&="},
            {OPCODE_AXORB,    u"^="},
            {OPCODE_AORB,     u"|="},
            {OPCODE_EQ,       u"=="},
            {OPCODE_NE,       u"!="},
            {OPCODE_GTE,      u">="},
            {OPCODE_LTE,      u"<="},
            {OPCODE_PWR,      u"**"},
            {OPCODE_INC,      u"++"},
            {OPCODE_DEC,      u"--"},
            {OPCODE_SHL,      u"<<"},
            {OPCODE_SHR,      u">>"},
            {OPCODE_ANDL,     u"&&"},
            {OPCODE_ORL,      u"||"},
            {OPCODE_COMMENT1, u"/*"},
            {OPCODE_COMMENT2, u"*/"},
            {OPCODE_COMMENTL, u"//"},  // The end for this is the newline.
    };
    static const opcode_cstr_map op_opcode_cstr_map_3 {
            {OPCODE_APWR,      u"**="},
            {OPCODE_ASHL,      u"<<="},
            {OPCODE_ASHR,      u">>="},
            {OPCODE_TRIPLEDOT, u"..."},
            {OPCODE_AANDL,     u"&&="},
            {OPCODE_AORL,      u"||="},
            {OPCODE_ANULLC,    u"?\?="},
            {OPCODE_EQE,       u"==="},
            {OPCODE_NEE,       u"!=="},
            {OPCODE_SHRU,      u">>>"},
    };
    static const opcode_cstr_map op_opcode_cstr_map_4 {
            {OPCODE_ASHRU, u">>>="},
    };

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
            static opcode_cstr_map op_opcode_cstr_map_0_builder {};
            op_opcode_cstr_map_0_builder.merge(*const_cast<opcode_cstr_map*>(&op_opcode_cstr_map_4));
            op_opcode_cstr_map_0_builder.merge(*const_cast<opcode_cstr_map*>(&op_opcode_cstr_map_3));
            op_opcode_cstr_map_0_builder.merge(*const_cast<opcode_cstr_map*>(&op_opcode_cstr_map_2));
            op_opcode_cstr_map_0_builder.merge(*const_cast<opcode_cstr_map*>(&op_opcode_cstr_map_1));

            static const opcode_cstr_map op_opcode_cstr_map_0 =
                    *const_cast<const opcode_cstr_map*>(&op_opcode_cstr_map_0_builder);

            return op_opcode_cstr_map_0;
        default:
            throw ERR_OPERATOR_INVALID_SIZE;
    }
}

const cstr_opcode_map& get_op_cstr_opcode_map (const uint8_t operator_size) {
    static const cstr_opcode_map op_cstr_opcode_map_1 {
            {u"?", OPCODE_QMARK},
            {u":", OPCODE_COLON},
            {u",", OPCODE_COMMA},
            {u".", OPCODE_DOT},
            {u"=", OPCODE_A},
            {u">", OPCODE_GT},
            {u"<", OPCODE_LT},
            {u"+", OPCODE_ADD},
            {u"-", OPCODE_SUB},
            {u"*", OPCODE_MUL},
            {u"/", OPCODE_DIV},
            {u"%", OPCODE_REM},
            {u"&", OPCODE_ANDB},
            {u"|", OPCODE_ORB},
            {u"^", OPCODE_XORB},
            {u"~", OPCODE_NOTB},
            {u"!", OPCODE_NOTL},
            {u"\"", OPCODE_QDOUBLE},
            {u"'", OPCODE_QSINGLE},
            {u"`", OPCODE_QTICK},
            {u"(", OPCODE_PARENTHESES1},
            {u")", OPCODE_PARENTHESES2},
            {u"[", OPCODE_BRACKET1},
            {u"]", OPCODE_BRACKET2},
            {u"{", OPCODE_BRACES1},
            {u"}", OPCODE_BRACES2},
    };
    static const cstr_opcode_map op_cstr_opcode_map_2 {
            {u"??", OPCODE_NULLC},
            {u"=>", OPCODE_ARROW},
            {u".?", OPCODE_DOTQMARK},
            {u"+=", OPCODE_AADD},
            {u"-=", OPCODE_ASUB},
            {u"*=", OPCODE_AMUL},
            {u"/=", OPCODE_ADIV},
            {u"%=", OPCODE_AREM},
            {u"&=", OPCODE_AANDB},
            {u"^=", OPCODE_AXORB},
            {u"|=", OPCODE_AORB},
            {u"==", OPCODE_EQ},
            {u"!=", OPCODE_NE},
            {u">=", OPCODE_GTE},
            {u"<=", OPCODE_LTE},
            {u"**", OPCODE_PWR},
            {u"++", OPCODE_INC},
            {u"--", OPCODE_DEC},
            {u"<<", OPCODE_SHL},
            {u">>", OPCODE_SHR},
            {u"&&", OPCODE_ANDL},
            {u"||", OPCODE_ORL},
            {u"/*", OPCODE_COMMENT1},
            {u"*/", OPCODE_COMMENT2},
            {u"//", OPCODE_COMMENTL},
    };
    static const cstr_opcode_map op_cstr_opcode_map_3 {
            {u"**=", OPCODE_APWR},
            {u"<<=", OPCODE_ASHL},
            {u">>=", OPCODE_ASHR},
            {u"...", OPCODE_TRIPLEDOT},
            {u"&&=", OPCODE_AANDL},
            {u"||=", OPCODE_AORL},
            {u"?\?=", OPCODE_ANULLC},
            {u"===", OPCODE_EQE},
            {u"!==", OPCODE_NEE},
            {u">>>", OPCODE_SHRU},
    };
    static const cstr_opcode_map op_cstr_opcode_map_4 {
            {u">>>=", OPCODE_ASHRU},
    };

    switch (operator_size) {
        case 4:
            return op_cstr_opcode_map_4;
        case 3:
            return op_cstr_opcode_map_3;
        case 2:
            return op_cstr_opcode_map_2;
        case 1:
            return op_cstr_opcode_map_1;
        case 0:
            static cstr_opcode_map op_cstr_opcode_map_0_builder {};
            op_cstr_opcode_map_0_builder.merge(*const_cast<cstr_opcode_map*>(&op_cstr_opcode_map_4));
            op_cstr_opcode_map_0_builder.merge(*const_cast<cstr_opcode_map*>(&op_cstr_opcode_map_3));
            op_cstr_opcode_map_0_builder.merge(*const_cast<cstr_opcode_map*>(&op_cstr_opcode_map_2));
            op_cstr_opcode_map_0_builder.merge(*const_cast<cstr_opcode_map*>(&op_cstr_opcode_map_1));

            static const cstr_opcode_map op_cstr_opcode_map_0 =
                    *const_cast<const cstr_opcode_map*>(&op_cstr_opcode_map_0_builder);
            return op_cstr_opcode_map_0;
        default:
            throw ERR_OPERATOR_INVALID_SIZE;
    }
}

const opcode_cstr_map& get_kw_opcode_cstr_map () {
    static const opcode_cstr_map kw_opcode_cstr_map {
            {OPCODE_BREAK,        u"break"},
            {OPCODE_CASE,         u"case"},
            {OPCODE_CATCH,        u"catch"},
            {OPCODE_CLASS,        u"class"},
            {OPCODE_CONST,        u"const"},
            {OPCODE_CONTINUE,     u"continue"},
            {OPCODE_DEBUGGER,     u"debugger"},
            {OPCODE_DEFAULT,      u"default"},
            {OPCODE_DELETE,       u"delete"},
            {OPCODE_DO,           u"do"},
            {OPCODE_ELSE,         u"else"},
            {OPCODE_EXPORT,       u"export"},
            {OPCODE_EXTENDS,      u"extends"},
            {OPCODE_FINALLY,      u"finally"},
            {OPCODE_FOR,          u"for"},
            {OPCODE_FUNCTION,     u"function"},
            {OPCODE_IF,           u"if"},
            {OPCODE_IMPORT,       u"import"},
            {OPCODE_IN,           u"in"},
            {OPCODE_OF,           u"of"},
            {OPCODE_INSTANCEOF,   u"instanceof"},
            {OPCODE_NEW,          u"new"},
            {OPCODE_RETURN,       u"return"},
            {OPCODE_SUPER,        u"super"},
            {OPCODE_SWITCH,       u"switch"},
            {OPCODE_THIS,         u"this"},
            {OPCODE_THROW,        u"throw"},
            {OPCODE_TRY,          u"try"},
            {OPCODE_TYPEOF,       u"typeof"},
            {OPCODE_VAR,          u"var"},
            {OPCODE_VOID,         u"void"},
            {OPCODE_WHILE,        u"while"},
            {OPCODE_WITH,         u"with"},
            {OPCODE_YIELD,        u"yield"},

            {OPCODE_ENUM,         u"enum"},

            {OPCODE_IMPLEMENTS,   u"implements"},
            {OPCODE_INTERFACE,    u"interface"},
            {OPCODE_LET,          u"let"},
            {OPCODE_PACKAGE,      u"package"},
            {OPCODE_PRIVATE,      u"private"},
            {OPCODE_PROTECTED,    u"protected"},
            {OPCODE_PUBLIC,       u"public"},
            {OPCODE_STATIC,       u"static"},

            {OPCODE_AWAIT,        u"await"},

            {OPCODE_ABSTRACT,     u"abstract"},
            {OPCODE_BOOLEAN,      u"boolean"},
            {OPCODE_BYTE,         u"byte"},
            {OPCODE_CHAR,         u"char"},
            {OPCODE_DOUBLE,       u"double"},
            {OPCODE_FINAL,        u"final"},
            {OPCODE_FLOAT,        u"float"},
            {OPCODE_GOTO,         u"goto"},
            {OPCODE_INT,          u"int"},
            {OPCODE_LONG,         u"long"},
            {OPCODE_NATIVE,       u"native"},
            {OPCODE_SHORT,        u"short"},
            {OPCODE_SYNCHRONIZED, u"synchronized"},
            {OPCODE_THROWS,       u"throws"},
            {OPCODE_TRANSIENT,    u"transient"},
            {OPCODE_VOLATILE,     u"volatile"},

            {OPCODE_NULL,         u"null"},
            {OPCODE_TRUE,         u"true"},
            {OPCODE_FALSE,        u"false"},
    };
    return kw_opcode_cstr_map;
}

const cstr_opcode_map& get_kw_cstr_opcode_map () {
    static const cstr_opcode_map kw_cstr_opcode_map {
            {u"break",        OPCODE_BREAK},
            {u"case",         OPCODE_CASE},
            {u"catch",        OPCODE_CATCH},
            {u"class",        OPCODE_CLASS},
            {u"const",        OPCODE_CONST},
            {u"continue",     OPCODE_CONTINUE},
            {u"debugger",     OPCODE_DEBUGGER},
            {u"default",      OPCODE_DEFAULT},
            {u"delete",       OPCODE_DELETE},
            {u"do",           OPCODE_DO},
            {u"else",         OPCODE_ELSE},
            {u"export",       OPCODE_EXPORT},
            {u"extends",      OPCODE_EXTENDS},
            {u"finally",      OPCODE_FINALLY},
            {u"for",          OPCODE_FOR},
            {u"function",     OPCODE_FUNCTION},
            {u"if",           OPCODE_IF},
            {u"import",       OPCODE_IMPORT},
            {u"in",           OPCODE_IN},
            {u"of",           OPCODE_OF},
            {u"instanceof",   OPCODE_INSTANCEOF},
            {u"new",          OPCODE_NEW},
            {u"return",       OPCODE_RETURN},
            {u"super",        OPCODE_SUPER},
            {u"switch",       OPCODE_SWITCH},
            {u"this",         OPCODE_THIS},
            {u"throw",        OPCODE_THROW},
            {u"try",          OPCODE_TRY},
            {u"typeof",       OPCODE_TYPEOF},
            {u"var",          OPCODE_VAR},
            {u"void",         OPCODE_VOID},
            {u"while",        OPCODE_WHILE},
            {u"with",         OPCODE_WITH},
            {u"yield",        OPCODE_YIELD},

            {u"enum",         OPCODE_ENUM},

            {u"implements",   OPCODE_IMPLEMENTS},
            {u"interface",    OPCODE_INTERFACE},
            {u"let",          OPCODE_LET},
            {u"package",      OPCODE_PACKAGE},
            {u"private",      OPCODE_PRIVATE},
            {u"protected",    OPCODE_PROTECTED},
            {u"public",       OPCODE_PUBLIC},
            {u"static",       OPCODE_STATIC},

            {u"await",        OPCODE_AWAIT},

            {u"abstract",     OPCODE_ABSTRACT},
            {u"boolean",      OPCODE_BOOLEAN},
            {u"byte",         OPCODE_BYTE},
            {u"char",         OPCODE_CHAR},
            {u"double",       OPCODE_DOUBLE},
            {u"final",        OPCODE_FINAL},
            {u"float",        OPCODE_FLOAT},
            {u"goto",         OPCODE_GOTO},
            {u"int",          OPCODE_INT},
            {u"long",         OPCODE_LONG},
            {u"native",       OPCODE_NATIVE},
            {u"short",        OPCODE_SHORT},
            {u"synchronized", OPCODE_SYNCHRONIZED},
            {u"throws",       OPCODE_THROWS},
            {u"transient",    OPCODE_TRANSIENT},
            {u"volatile",     OPCODE_VOLATILE},

            {u"null",         OPCODE_NULL},
            {u"true",         OPCODE_TRUE},
            {u"false",        OPCODE_FALSE},
    };
    return kw_cstr_opcode_map;
}
