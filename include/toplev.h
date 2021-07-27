#ifndef M6_TOPLEV_H
#define M6_TOPLEV_H

#include <cinttypes>
#include <errors.h>
#include <opcodes.h>
#include <config.h>
#include <io.h>

#define SUCCESS 0
#define NOT_FOUND -1

#define LTS_ \
    while (true) { \
        try {

#ifdef LOG_ERRORS
#define _LTS \
            break; \
        } catch (const int e) { \
            _L("%s\n", errors[e]); \
            _X(); \
            break; \
        } \
    }
#else
#define _LTS \
            break; \
        } catch (const err_t e) {} \
    }
#endif

typedef int return_status_t;

inline constexpr unsigned char operator "" _uc (unsigned long long arg) noexcept {
    return static_cast <unsigned char> (arg);
}

inline constexpr unsigned char operator "" _c (unsigned long long arg) noexcept {
    return static_cast <char> (arg);
}

#endif
