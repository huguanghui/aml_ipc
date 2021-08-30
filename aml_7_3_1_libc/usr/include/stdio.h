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

__BEGIN_NAMESPACE_STD
#ifndef __USE_FILE_OFFSET64
typedef _G_fpos_t fpos_t;
#else
typedef _G_fpos64_t fpos_t;
#endif
__END_NAMESPACE_STD

#ifdef __USE_LARGEFILE64
typedef _G_fpos64_t fpos64_t;
#endif // __USE_LARGEFILE64

#define _IOFBF 0
#define _IOLBF 1
#define _IONBF 2

#ifndef BUFSIZ
#define BUFSIZ _IO_BUFSIZ
#endif // !BUFSIZ

#ifndef EOF
#define EOF (-1)
#endif // !EOF

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#ifdef __USE_GNU
#define SEEK_DATA 3
#define SEEK_HOLE 4
#endif // __USE_GNU

#if defined __USE_MISC || defined __USE_XOPEN
#define P_tmpdir "/tmp"
#endif

#include <bits/stdio_lim.h>

extern struct _IO_FILE* stdin;
extern struct _IO_FILE* stdout;
extern struct _IO_FILE* stderr;

#define stdin stdin
#define stdout stdout
#define stderr stderr

__BEGIN_NAMESPACE_STD
extern int remove(const char* __filename) __THROW;
extern int rename(const char* __old, const char* __new) __THROW;
__END_NAMESPACE_STD

#ifdef __USE_ATFILE
extern int renameat(int __oldfd, const char* __old, int __newfd, const char* __new) __THROW;
#endif // __USE_ATFILE

__BEGIN_NAMESPACE_STD
#ifndef __USE_FILE_OFFSET64
extern FILE* tmpfile(void) __wur;
#else
#ifdef __REDIRECT
extern FILE* __REDIRECT(tmpfile, (void), tmpfile64) __wur;
#else
#define tmpfile tmpfile64
#endif
#endif // !__USE_FILE_OFFSET64

#ifdef __USE_LARGEFILE64
extern FILE* tmpfile64(void) __wur;
#endif // __USE_LARGEFILE64

extern char* tmpnam(char* __s) __THROW __wur;
__END_NAMESPACE_STD

#ifdef __USE_MISC
extern char* tmpnam_r(char* __s) __THROW __wur;
#endif // __USE_MISC

#if defined __USE_MISC || defined __USE_XOPEN
extern char* tempnam(const char* __dir, const char* __pfx) __THROW __attribute_malloc__ __wur;
#endif

__BEGIN_NAMESPACE_STD
extern int fclose(FILE* __stream);
extern int fflush(FILE* __stream);
__END_NAMESPACE_STD

#ifdef __USE_MISC
extern int fflush_unlocked(FILE* __stream);
#endif

#ifdef __USE_GNU
extern int fcloseall(void);
#endif

__END_DECLS

#endif

#endif // !_STDIO_H
