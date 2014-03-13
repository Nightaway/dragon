#ifndef __STDINI_H__
#define __STDINT_H__

#include <stddef.h>
#include <stdio.h>

#if defined(_WIN32) && !define(__MINGW32__)

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;

#else

#include <stdint.h>

#endif
