// Copyright (c) 2021 Sri Lakshmi Kanthan P
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT
//
// Original at:
// https://github.com/srilakshmikanthanp/ANSI/blob/master/include/srilakshmikanthanp/ANSI.hpp
#ifndef M6_COLORS_H
#define M6_COLORS_H

#define _USE_MATH_DEFINES

#ifdef __linux__

#include <iostream>
#include <string>
#include <sstream>
#include <math.h>

int ANSI__init()
{
    return 0;
}

#elif _COLORS_WIN32

#include <iostream>
#include <string>
#include <sstream>
#include <windows.h>
#include <math.h>

int ANSI__init()
{
    HANDLE StdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    if (StdOut != INVALID_HANDLE_VALUE)
    {
        DCOLORS_WORD mode = 0;
        if (GetConsoleMode(StdOut, &mode))
        {
            mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            if (SetConsoleMode(StdOut, mode))
            {
                return 0;
            }
        }
    }

    return GetLastError();
}

#endif

/**
 * @brief Some macros that makes code smaller
 */
#define COLORS_TEMPLATE template <class CharT, class TraitsT>
#define COLORS_OSTREAM std::basic_ostream<CharT, TraitsT>
#define COLORS_STRING std::basic_string<CharT>
#define COLORS_W(str) [](std::string arg) {       \
    return COLORS_STRING(arg.begin(), arg.end()); \
}(std::string(str))

/**
 * @namespace srilakshmikanthanp::ansi
 * @brief  contains ansi escape sequence
 */
namespace srilakshmikanthanp::ansi
{
    /**
     * @brief error code, Make sure it is zero
     * especially for windows platforms.
     */
    const int init_error = ANSI__init();

    /**
     * @brief ansi to string
     */
    std::string str(std::ostream &(*manip)(std::ostream &))
    {
        std::ostringstream stream("");
        stream << manip;
        return stream.str();
    }

    /**
     * @brief ansi to str
     *
     * @tparam T type
     * @param obj object
     * @return COLORS_STRING
     */
    template <class T>
    std::string str(T obj)
    {
        std::ostringstream stream("");
        stream << obj;
        return stream.str();
    }

    /**
     * @brief Moves the cursor to row n, column m.
     */
    struct cursor
    {
    private:
        int n, m;

    public:
        cursor(int n, int m) : n{n}, m{m}
        {
        }

        COLORS_TEMPLATE
        friend COLORS_OSTREAM &operator<<(COLORS_OSTREAM &os, const cursor &cur)
        {
            return os << COLORS_W("\033[") << cur.n << COLORS_W(";") << cur.m << COLORS_W("H");
        }
    };

    /**
     * @brief Clears part of the screen. If n is 0 (or missing),
     * clear from cursor to end of screen.
     * If n is 1, clear from cursor to beginning of the screen.
     * If n is 2, clear entire screen (and moves cursor to upper
     * left on DOS ansi.SYS). If n is 3, clear entire screen and
     * delete all lines saved in the scrollback buffer
     * (this feature was added for xterm and is supported by
     * other terminal applications).
     */
    struct clrscr
    {
    private:
        int n;

    public:
        clrscr(int n) : n{n}
        {
        }

        COLORS_TEMPLATE
        friend COLORS_OSTREAM &operator<<(COLORS_OSTREAM &os, const clrscr &dis)
        {
            return os << COLORS_W("\033[") << dis.n << COLORS_W("J");
        }
    };

    /**
     * @brief Erases part of the line. If n is 0 (or missing),
     * clear from cursor to the end of the line. If n is 1,
     * clear from cursor to beginning of the line. If n is 2,
     * clear entire line. Cursor position does not change.
     */
    struct clrlin
    {
    private:
        int n;

    public:
        clrlin(int n) : n{n}
        {
        }

        COLORS_TEMPLATE
        friend COLORS_OSTREAM &operator<<(COLORS_OSTREAM &os, const clrlin &lin)
        {
            return os << COLORS_W("\033[") << lin.n << COLORS_W("K");
        }
    };

    /**
     * @brief Scroll whole page up by n (default 1) lines.
     * New lines are added at the bottom.
     */
    struct scrollup
    {
    private:
        int n;

    public:
        scrollup(int n) : n{n}
        {
        }

        COLORS_TEMPLATE
        friend COLORS_OSTREAM &operator<<(COLORS_OSTREAM &os, const scrollup &ref)
        {
            return os << COLORS_W("\033[") << ref.n << COLORS_W("S");
        }
    };

    /**
     * @brief Scroll whole page down by n (default 1) lines.
     * New lines are added at the top.
     */
    struct scrolldn
    {
    private:
        int n;

    public:
        scrolldn(int n) : n{n}
        {
        }

        COLORS_TEMPLATE
        friend COLORS_OSTREAM &operator<<(COLORS_OSTREAM &os, const scrolldn &ref)
        {
            return os << COLORS_W("\033[") << ref.n << COLORS_W("T");
        }
    };

    /**
     * @brief Save Current Cursor Position
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &savepos(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[s");
    }

    /**
     * @brief Restore Saved Cursor Position
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &loadpos(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[u");
    }

    /**
     * @brief Shows the cursor, from the VT220.
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &showcursor(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[?25h");
    }

    /**
     * @brief Restore Saved Cursor Position
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &hidecursor(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[?25l");
    }

    /**
     * @brief reset manipulator
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &reset(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[0m");
    }

    /**
     * @brief Bold or increased intensity
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &bold(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[1m");
    }

    /**
     * @brief Faint, decreased intensity, or dim
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &faint(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[2m");
    }

    /**
     * @brief italic
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &italic(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[3m");
    }

    /**
     * @brief 	Underline
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &underline(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[4m");
    }

    /**
     * @brief Slow blink
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &blink(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[5m");
    }

    /**
     * @brief invert
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &invert(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[7m");
    }

    /**
     * @brief Conceal
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &conceal(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[8m");
    }

    /**
     * @brief strike
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &strike(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[9m");
    }

    /**
     * @brief font
     */
    struct font
    {
    private:
        int n;

    public:
        font(int n) : n{n}
        {
        }

        COLORS_TEMPLATE
        friend COLORS_OSTREAM &operator<<(COLORS_OSTREAM &os, const font &ref)
        {
            if (ref.n >= 10 && ref.n <= 20)
            {
                return os << COLORS_W("\033[") << ref.n << COLORS_W("m");
            }
            else
            {
                return os;
            }
        }
    };

    /**
     * @brief Normal intensity
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &normalintensity(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[22m");
    }

    /**
     * @brief not italic
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &notitalic(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[23m");
    }

    /**
     * @brief not underlined
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &notunderline(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[24m");
    }

    /**
     * @brief not blinked
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &notblink(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[25m");
    }

    /**
     * @brief proportional space
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &prospace(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[26m");
    }

    /**
     * @brief not invert
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &notinvert(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[27m");
    }

    /**
     * @brief not conceal
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &notconceal(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[28m");
    }

    /**
     * @brief not strkie
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &notstrike(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[29m");
    }

    /**
     * @brief Foreground black
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &fg_black(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[30m");
    }

    /**
     * @brief Foreground red
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &fg_red(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[31m");
    }

    /**
     * @brief Foreground green
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &fg_green(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[32m");
    }

    /**
     * @brief Foreground yellow
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &fg_yellow(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[33m");
    }

    /**
     * @brief Foreground blue
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &fg_blue(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[34m");
    }

    /**
     * @brief Foreground magenta
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &fg_magenta(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[35m");
    }

    /**
     * @brief Foreground cyan
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &fg_cyan(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[36m");
    }

    /**
     * @brief Foreground white
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &fg_white(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[37m");
    }

    /**
     * @brief Foreground brightblack
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &fg_brightblack(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[90m");
    }

    /**
     * @brief Foreground brightred
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &fg_brightred(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[91m");
    }

    /**
     * @brief Foreground brightgreen
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &fg_brightgreen(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[92m");
    }

    /**
     * @brief Foreground brightyellow
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &fg_brightyellow(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[93m");
    }

    /**
     * @brief Foreground brightblue
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &fg_brightblue(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[94m");
    }

    /**
     * @brief Foreground brightmagenta
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &fg_brightmagenta(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[95m");
    }

    /**
     * @brief Foreground brightcyan
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &fg_brightcyan(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[96m");
    }

    /**
     * @brief Foreground brightwhite
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &fg_brightwhite(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[97m");
    }

    /**
     * @brief Foreground default
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &fg_default(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[39m");
    }

    /**
     * @brief Foreground color
     */
    struct fg_color
    {
    private:
        int n, r, g, b;

    public:
        fg_color(int n) : n{n}, r{-1}, g{-1}, b{-1}
        {
        }

        fg_color(int r, int g, int b) : n{-1}, r{r}, g{g}, b{b}
        {
        }

        COLORS_TEMPLATE
        friend COLORS_OSTREAM &operator<<(COLORS_OSTREAM &os, const fg_color &ref)
        {
            if (ref.n >= 0 && ref.n <= 255)
            {
                return os << COLORS_W("\033[38;5;") << ref.n << COLORS_W("m");
            }
            else if (ref.r >= 0 && ref.r <= 255 && ref.g >= 0 && ref.g <= 255 && ref.b >= 0 && ref.b <= 255)
            {
                return os << COLORS_W("\033[38;2;") << ref.r << COLORS_W(";") << ref.g << COLORS_W(";") << ref.b << COLORS_W("m");
            }
            else
            {
                return os;
            }
        }
    };

    /**
     * @brief background black
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &bg_black(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[40m");
    }

    /**
     * @brief background red
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &bg_red(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[41m");
    }

    /**
     * @brief background green
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &bg_green(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[42m");
    }

    /**
     * @brief background yellow
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &bg_yellow(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[43m");
    }

    /**
     * @brief background blue
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &bg_blue(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[44m");
    }

    /**
     * @brief background magenta
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &bg_magenta(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[45m");
    }

    /**
     * @brief background cyan
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &bg_cyan(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[46m");
    }

    /**
     * @brief background white
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &bg_white(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[47m");
    }

    /**
     * @brief background brightblack
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &bg_brightblack(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[100m");
    }

    /**
     * @brief background brightred
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &bg_brightred(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[101m");
    }

    /**
     * @brief background brightgreen
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &bg_brightgreen(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[102m");
    }

    /**
     * @brief background brightyellow
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &bg_brightyellow(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[103m");
    }

    /**
     * @brief background brightblue
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &bg_brightblue(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[104m");
    }

    /**
     * @brief background brightmagenta
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &bg_brightmagenta(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[105m");
    }

    /**
     * @brief background brightcyan
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &bg_brightcyan(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[96m");
    }

    /**
     * @brief background brightwhite
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &bg_brightwhite(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[97m");
    }

    /**
     * @brief background default
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &bg_default(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[49m");
    }

    /**
     * @brief background color
     */
    struct bg_color
    {
    private:
        int n, r, g, b;

    public:
        bg_color(int n) : n{n}, r{-1}, g{-1}, b{-1}
        {
        }

        bg_color(int r, int g, int b) : n{-1}, r{r}, g{g}, b{b}
        {
        }

        COLORS_TEMPLATE
        friend COLORS_OSTREAM &operator<<(COLORS_OSTREAM &os, const bg_color &ref)
        {
            if (ref.n >= 0 && ref.n <= 255)
            {
                return os << COLORS_W("\033[48;5;") << ref.n << COLORS_W("m");
            }
            else if (ref.r >= 0 && ref.r <= 255 && ref.g >= 0 && ref.g <= 255 && ref.b >= 0 && ref.b <= 255)
            {
                return os << COLORS_W("\033[48;2;") << ref.r << COLORS_W(";") << ref.g << COLORS_W(";") << ref.b << COLORS_W("m");
            }
            else
            {
                return os;
            }
        }
    };

    /**
     * @brief not proprotional space
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &notprospace(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[50m");
    }

    /**
     * @brief framed
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &framed(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[51m");
    }

    /**
     * @brief encircled
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &encircled(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[52m");
    }

    /**
     * @brief Overlined
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &overlined(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[53m");
    }

    /**
     * @brief not framed or encircled
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &notemoj(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[54m");
    }

    /**
     * @brief not overlined
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &notoverlined(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[54m");
    }

    /**
     * @brief background default
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &defaultunderlinecolor(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[49m");
    }

    /**
     * @brief background color
     */
    struct underlinecolor
    {
    private:
        int n, r, g, b;

    public:
        underlinecolor(int n) : n{n}, r{-1}, g{-1}, b{-1}
        {
        }

        underlinecolor(int r, int g, int b) : n{-1}, r{r}, g{g}, b{b}
        {
        }

        COLORS_TEMPLATE
        friend COLORS_OSTREAM &operator<<(COLORS_OSTREAM &os, const underlinecolor &ref)
        {
            if (ref.n >= 0 && ref.n <= 255)
            {
                return os << COLORS_W("\033[58;5;") << ref.n << COLORS_W("m");
            }
            else if (ref.r >= 0 && ref.r <= 255 && ref.g >= 0 && ref.g <= 255 && ref.b >= 0 && ref.b <= 255)
            {
                return os << COLORS_W("\033[58;2;") << ref.r << COLORS_W(";") << ref.g << COLORS_W(";") << ref.b << COLORS_W("m");
            }
            else
            {
                return os;
            }
        }
    };

    /**
     * @brief super script
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &supscript(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[73m");
    }

    /**
     * @brief sub script
     */
    COLORS_TEMPLATE
    COLORS_OSTREAM &subscript(COLORS_OSTREAM &os)
    {
        return os << COLORS_W("\033[74m");
    }

    /**
     * @brief colorize the given text
     */
    class colorize
    {
    private:
        double fq1, fq2, fq3, amp, cen, ps1, ps2, ps3;
        std::string str;

    public:
        /**
         * @brief Construct a new colorize object
         *
         * @param str string
         */
        colorize(std::string str) : str(str)
        {
            fq1 = 2 * M_PI / str.size();
            fq2 = 2 * M_PI / str.size();
            fq3 = 2 * M_PI / str.size();
            amp = 127;
            cen = 128;
            ps1 = 0 * M_PI/3;
            ps2 = 2 * M_PI/3;
            ps3 = 4 * M_PI/3;
        }

        /**
         * @brief output
         *
         * @param os stream
         * @param obj obj
         * @return COLORS_OSTREAM& stream
         */
        COLORS_TEMPLATE
        friend COLORS_OSTREAM& operator<<(COLORS_OSTREAM& os, const colorize &obj)
        {
            int r = 0, g = 0, b = 0, v = 0;

            for(std::string::size_type i = 0; i < obj.str.size(); ++i)
            {
                if(!std::isspace(obj.str[i]))
                {
                    r  =  std::sin(obj.fq1 * v + obj.ps1) * obj.amp + obj.cen;
                    g  =  std::sin(obj.fq2 * v + obj.ps2) * obj.amp + obj.cen;
                    b  =  std::sin(obj.fq3 * v + obj.ps3) * obj.amp + obj.cen;
                    v  =  v + 1;
                    os << fg_color(r, g, b) << obj.str[i];
                }
                else
                {
                    os << obj.str[i];
                }
            }

            return os << reset;
        }
    };
} // namespace srilakshmikanthanp::ansi

/**
 * @brief undef macros
 */
#undef _USE_MATH_DEFINES
#undef COLORS_TEMPLATE
#undef COLORS_OSTREAM
#undef COLORS_STRING
#undef COLORS_W

#endif
