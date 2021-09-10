#ifndef M6_TOKENIZER_H
#define M6_TOKENIZER_H

#include <LiteralProcessor.h>

class Tokenizer : public LiteralProcessor {
public:
    explicit Tokenizer (int log_handler (const char*, ...));

    Token tokenize (const char* file_name);

    Token tokenize (const std::string& str);

    Token tokenize (const std::string::const_iterator& begin, const std::string::const_iterator& end);

protected:
    bool process_next_token ();

    std::string content;
};


#endif
