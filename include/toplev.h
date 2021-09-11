#ifndef M6_TOPLEV_H
#define M6_TOPLEV_H

// For opcodes
#include <map>
#include <cinttypes>
#include <cstring>

// For token handlers
#include <fstream>
#include <algorithm>  // Includes <vector> as well.
#include <codecvt>
#include <locale>

// For everything
#include <errors.h>
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
        } catch (const int64_t e) { \
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

typedef int64_t return_status_t;

inline constexpr unsigned char operator "" _uc (unsigned long long arg) noexcept {
    return static_cast <unsigned char> (arg);
}

inline constexpr unsigned char operator "" _c (unsigned long long arg) noexcept {
    return static_cast <char> (arg);
}

template <typename T>
std::string toUTF8(const std::basic_string<T, std::char_traits<T>, std::allocator<T>>& source) {
    std::string result;

    std::wstring_convert<std::codecvt_utf8_utf16<T>, T> convertor;
    result = convertor.to_bytes(source);

    return result;
}

template <typename T>
void fromUTF8 (const std::string& source, std::basic_string<T, std::char_traits<T>, std::allocator<T>>& result) {
    std::wstring_convert<std::codecvt_utf8_utf16<T>, T> convertor;
    result = convertor.from_bytes(source);
}

#endif
