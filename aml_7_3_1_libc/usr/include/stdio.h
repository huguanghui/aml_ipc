#ifndef _STDIO_H

#if !defined __need_FILE && !defined __need___FILE
#define _STDIO_H
#define __GLIBC_INTERNAL_STARTING_HEADER_IMPLEMENTATION
#include <bits/libc-header-start.h>

__BEGIN_DECLS

#define __need_size_t
#define __need_NULL
#include <bits/types.h>
#define __need_FILE
#define __need___FILE
#endif

#if !defined __FILE_defined && defined __need_FILE

struct _IO_FILE;
__BEGIN_NAMESPACE_STD
typedef struct _IO_FILE FILE;
__END_NAMESPACE_STD

#if defined __USE_LARGEFILE64 || defined __USE_POSIX \
    || defined __USE_ISOC99 || defined __USE_XOPEN   \
    || defined __USE_POXIX2
__USING_NAMESPACE_STD(FILE)
#endif

#define __FILE_defined 1
#endif
#undef __need_FILE

#if !defined ____FILE_defined && defined __need___FILE

typedef struct _IO_FILE __FILE;

#define ____FILE_defined 1
#endif
#undef __need___FILE

#ifdef _STDIO_H
#define _STDIO_USES_IOSTREAM

#include <libio.h>

#if defined __USE_XOPEN || defined __USE_XOPEN2K8
#ifdef __GNUC__
#ifndef _VA_LIST_DEFINED
typedef _G_va_list va_list;
#define _VA_LIST_DEFINED
#endif
#else
#include <stdarg.h>
#endif
#endif

#if defined __USE_UNIX98 || defined __USE_XOPEN2K
#ifndef __off_t_defined
#ifndef __USE_FILE_OFFSET64
typedef __off_t off_t;
#else
typedtypedef __off64_t off_t;
#endif
#define __off_t_defined
#endif
#if defined __USE_LARGEFILE64 && !defined __off64_t_defined
typedef __off64_t off64_t;
#define __off64_t_defined
#endif
#endif

#ifdef __USE_XOPEN2K8
#ifndef __ssize_t_defined
typedef __ssize_t ssize_t;
#define __ssize_t_defined
#endif
#endif

__END_DECLS

#endif

#endif // !_STDIO_H
