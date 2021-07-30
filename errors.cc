#include <errors.h>

const char errors[ERR_COUNT + 1][100] = {
        "",
        "[ERROR] Input file stream failed to read the file %s.",
        "[ERROR] Wrong number of arguments. Expected only one argument, the file name to interpret.",
        "[ERROR] A syntax error has been found while tokenizing.",
        "[ERROR] The size of the operator needs to be between 1 and 4, or 0 for checking all operators."
};
