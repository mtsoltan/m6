#ifndef M6_LITERALPROCESSOR_H
#define M6_LITERALPROCESSOR_H

#include <cstring>

#include <TokenTypeChecker.h>

class LiteralProcessor : public TokenTypeChecker {
public:
    using TokenTypeChecker::TokenTypeChecker;  // Inherit the constructor.
protected:
    bool process_number_literal();
    bool process_identifier();
    bool process_keyword(opcode_t memoized);
};


#endif
