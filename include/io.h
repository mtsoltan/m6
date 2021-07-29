#ifndef M6_IO_H
#define M6_IO_H

#include <config.h>

int null_io_handler (const char* a, ...);

#ifdef USE_STDIO

#include <cstdio>

#define _X()   fflush(stdin)
#define _F     sprintf
#define _L     printf
#endif

#ifndef _X
#define _X() do {} while (0)
#endif
#ifndef _F
#define _F null_io_handler
#endif
#ifndef _L
#define _L null_io_handler
#endif

#endif
