#include <opcodes.h>

/**
 * Produces a const char[KEYWORD_SIZE] from a given enum OPCODE_* keyword opcode.
 * If the next paramter is provided, the opcode parameter is ignored completely.
 *
 * The next paramter causes it to return the nth element of the map, which is useful for iteration.
 *
 * @param keyword_opcode
 * @param index
 * @return
 */
std::map<opcode_t, const char*>* get_kw_map () {
    static const std::map<opcode_t, const char*> kw_map {
            {OPCODE_BREAK, "break"},
            {OPCODE_CASE, "case"},
            {OPCODE_CATCH, "catch"},
            {OPCODE_CLASS, "class"},
            {OPCODE_CONST, "const"},
            {OPCODE_CONTINUE, "continue"},
            {OPCODE_DEBUGGER, "debugger"},
            {OPCODE_DEFAULT, "default"},
            {OPCODE_DELETE, "delete"},
            {OPCODE_DO, "do"},
            {OPCODE_ELSE, "else"},
            {OPCODE_EXPORT, "export"},
            {OPCODE_EXTENDS, "extends"},
            {OPCODE_FINALLY, "finally"},
            {OPCODE_FOR, "for"},
            {OPCODE_FUNCTION, "function"},
            {OPCODE_IF, "if"},
            {OPCODE_IMPORT, "import"},
            {OPCODE_IN, "in"},
            {OPCODE_OF, "of"},
            {OPCODE_INSTANCEOF, "instanceof"},
            {OPCODE_NEW, "new"},
            {OPCODE_RETURN, "return"},
            {OPCODE_SUPER, "super"},
            {OPCODE_SWITCH, "switch"},
            {OPCODE_THIS, "this"},
            {OPCODE_THROW, "throw"},
            {OPCODE_TRY, "try"},
            {OPCODE_TYPEOF, "typeof"},
            {OPCODE_VAR, "var"},
            {OPCODE_VOID, "void"},
            {OPCODE_WHILE, "while"},
            {OPCODE_WITH, "with"},
            {OPCODE_YIELD, "yield"},

            {OPCODE_ENUM, "enum"},

            {OPCODE_IMPLEMENTS, "implements"},
            {OPCODE_INTERFACE, "interface"},
            {OPCODE_LET, "let"},
            {OPCODE_PACKAGE, "package"},
            {OPCODE_PRIVATE, "private"},
            {OPCODE_PROTECTED, "protected"},
            {OPCODE_PUBLIC, "public"},
            {OPCODE_STATIC, "static"},

            {OPCODE_AWAIT, "await"},

            {OPCODE_ABSTRACT, "abstract"},
            {OPCODE_BOOLEAN, "boolean"},
            {OPCODE_BYTE, "byte"},
            {OPCODE_CHAR, "char"},
            {OPCODE_DOUBLE, "double"},
            {OPCODE_FINAL, "final"},
            {OPCODE_FLOAT, "float"},
            {OPCODE_GOTO, "goto"},
            {OPCODE_INT, "int"},
            {OPCODE_LONG, "long"},
            {OPCODE_NATIVE, "native"},
            {OPCODE_SHORT, "short"},
            {OPCODE_SYNCHRONIZED, "synchronized"},
            {OPCODE_THROWS, "throws"},
            {OPCODE_TRANSIENT, "transient"},
            {OPCODE_VOLATILE, "volatile"},

            {OPCODE_NULL, "null"},
            {OPCODE_TRUE, "true"},
            {OPCODE_FALSE, "false"},
    };

    return const_cast<std::map<opcode_t, const char *> *>(&kw_map);
}
