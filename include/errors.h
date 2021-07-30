#ifndef M6_ERRORS_H
#define M6_ERRORS_H


#define ERR_COUNT 4

#define ERR_IFSTREAM_FAILED         1
#define ERR_INVALID_ARGC            2
#define ERR_TOKENIZING_SYNTAX_ERROR 3
#define ERR_OPERATOR_INVALID_SIZE   4


extern const char errors[ERR_COUNT + 1][100];


#endif
