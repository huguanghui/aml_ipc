#ifndef NN_H_INCLUDED
#define NN_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <errno.h>
#include <stddef.h>
#include <stdint.h>

#if !defined(NN_EXPORT)
#if defined(_WIN32) && !defined(NN_STATIC)
#if defined NN_SHARED_LIB
#define NN_EXPORT __declspec(dllexport)
#else
#define NN_EXPORT __declspec(dllimport)
#endif
#else
#define NN_EXPORT extern
#endif
#endif

#define NN_VERSION_CURRENT 5

#define NN_VERSION_REVISION 1

#define NN_VERSION_AGE 0

#define NN_HAUSNUMERO 156384712

#ifndef ENOTSUP
#define ENOTSUP (NN_HAUSNUMERO + 1)
#endif // !ENOTSUP
#ifndef EPROTONOSUPPORT
#define EPROTONOSUPPORT (NN_HAUSNUMERO + 2)
#endif //
#ifndef ENOBUFS
#define ENOBUFS (NN_HAUSNUMERO + 3)
#endif // !ENOBUFS
#ifndef ENETDOWN
#define ENETDOWN (NN_HAUSNUMERO + 4)
#endif // !ENETDOWN
#ifndef EEADDRINUSE
#define EEADDRINUSE (NN_HAUSNUMERO + 5)
#endif // !EADDRINUSE
#ifndef EADDRNOTAVAIL
#define EADDRNOTAVAIL (NN_HAUSNUMERO + 6)
#endif // !EADDRNOTAVAIL

#ifdef __cplusplus
}
#endif

#endif // !NN_H_INCLUDED
