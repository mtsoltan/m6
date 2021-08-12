#ifndef M6_TOKENIZER_H
#define M6_TOKENIZER_H

#include <LiteralProcessor.h>

class Tokenizer : public LiteralProcessor {
public:
    explicit Tokenizer (int log_handler (const char*, ...));

    std::vector<ValueToken*>& tokenize (const char* file_name);

    std::vector<ValueToken*>& tokenize (const std::string& str);

    std::vector<ValueToken*>& tokenize (const std::string::const_iterator& begin, const std::string::const_iterator& end);

    std::vector<std::string>& get_identifier_stack ();

protected:
    bool process_next_token ();
};


#endif
