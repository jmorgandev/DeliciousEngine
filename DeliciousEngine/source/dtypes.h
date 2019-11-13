#ifndef DTYPES_H
#define DTYPES_H

typedef unsigned __int64 uint64, qword;
typedef __int64 int64;
typedef unsigned __int32 uint32, dword;
typedef __int32 int32;
typedef unsigned __int16 uint16, word;
typedef __int16 int16;
typedef unsigned __int8 uint8, uchar, byte;
typedef __int8 int8;

#ifdef BUILD_x64
typedef int64 sint;
typedef uint64 uint;
#elif BUILD_x86
typedef int32 sint;
typdef uint32 uint;
#else
typedef int sint;
typedef unsigned int uint;
#endif

typedef const char *cstring;

#endif