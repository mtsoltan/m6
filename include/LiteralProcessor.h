#ifndef M6_LITERALPROCESSOR_H
#define M6_LITERALPROCESSOR_H

#include <cstring>

#include <TokenTypeChecker.h>

#define LOG_EXPECTING_BUFFER_N ((uint8_t) 2)
#define EXPECTING_BUFFER_N ((uint8_t) 1 << LOG_EXPECTING_BUFFER_N)
class LiteralProcessor : public TokenTypeChecker {
public:
    using TokenTypeChecker::TokenTypeChecker;  // Inherit the constructor.
protected:
    token_type_t expecting[EXPECTING_BUFFER_N] {};
    uint8_t expecting_iterator = 0;

    void expect (token_type_t t);

    token_type_t unexpect ();

    bool process_number_literal ();

    bool process_identifier ();

    bool process_keyword (opcode_t memoized);
};


#endif
