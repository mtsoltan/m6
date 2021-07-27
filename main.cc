#include <Tokenizer.h>

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
