#ifndef M6_ERRORS_H
#define M6_ERRORS_H


#define ERR_COUNT 3

#define ERR_IFSTREAM_FAILED         1
#define ERR_INVALID_ARGC            2
#define ERR_TOKENIZING_SYNTAX_ERROR 3


extern char errors[ERR_COUNT + 1][100];


#endif
