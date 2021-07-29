#ifndef M6_TOKENTYPECHECKER_H
#define M6_TOKENTYPECHECKER_H

#include <fstream>
#include <algorithm>

#include <toplev.h>
#include <Token.h>

// NO_INCREMENT is used to signal that the token iterator has reached the character after this token ended,
// and does not need to be incremented to reach it.
//
// If code parses till the last character and stops (without incrementing), we increment.
// If code parses the last character and increments (as a part of a loop procedure for example),
// then we specify NO_INCREMENT and put a comment elaborating further.
#define NO_INCREMENT ;

#define TOKEN_VECTOR_RESERVE         0x00'04'00

class TokenTypeChecker {
public:
    explicit TokenTypeChecker (int log_handler (const char*, ...));

protected:
    int (* log_handler) (const char*, ...);

    std::vector<Token*> token_vector;

    std::string tokenizer_string;
    std::string::iterator tokenizer_iterator;


    bool process_symbol ();


    bool next_token_is_number ();

    opcode_t next_token_is_keyword ();

    int64_t get_char_offset ();


};

#endif
