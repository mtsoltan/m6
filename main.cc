#include <Tokenizer.h>

// TODO: https://github.com/mtsoltan/m6/issues/14
// TODO: https://github.com/mtsoltan/m6/issues/17
int main (int argc, const char** argv) {
    LTS_

            if (argc > 2) {
                throw ERR_INVALID_ARGC;
            }

            const char* file_name = argv[1];

            Tokenizer tokenizer(_L);

            tokenizer.tokenize(file_name);

    _LTS
    return 0;
}
