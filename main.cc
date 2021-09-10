#include <Tokenizer.h>
#include <iostream>

// TODO: https://github.com/mtsoltan/m6/issues/14
// TODO: https://github.com/mtsoltan/m6/issues/17
int main (int argc, const char** argv) {
    LTS_

            if (argc > 2) {
                throw ERR_INVALID_ARGC;
            }

            const char* file_name = argv[1];

            auto tokenizer = Tokenizer(_L);

            tokenizer.tokenize(file_name);

            std::cout << tokenizer.colorized_output();

    _LTS
    return 0;
}
