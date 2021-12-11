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
#ifndef ECONNREFUSED
#define ECONNREFUSED (NN_HAUSNUMERO + 7)
#endif // !ECONNREFUSED
#ifndef EINPROGRESS
#define EINPROGRESS (NN_HAUSNUMERO + 8)
#endif // !EINPROGRESS
#ifndef ENOTSOCK
#define ENOTSOCK (NN_HAUSNUMERO + 9)
#endif // !ENOTSOCK
#ifndef EAFNOSUPPORT
#define EAFNOSUPPORT (NN_HAUSNUMERO + 10)
#endif // !EAFNOSUPPORT
#ifndef EPROTO
#define EPROTO (NN_HAUSNUMERO + 11)
#endif // !EPROTO
#ifndef EAGAIN
#define EAGAIN (NN_HAUSNUMERO + 12)
#endif // !EAGAIN
#ifndef EBADF
#define EBADF (NN_HAUSNUMERO + 13)
#endif // !EBADF
#ifndef EINVAL
#define EINVAL (NN_HAUSNUMERO + 14)
#endif // !EINVAL
#ifndef EMFILE
#define EMFILE (NN_HAUSNUMERO + 15)
#endif // !EMFILE
#ifndef EFAULT
#define EFAULT (NN_HAUSNUMERO + 16)
#endif // !EFAULT
#ifndef EACCES
#define EACCES (NN_HAUSNUMERO + 17)
#endif // !EACCES
#ifndef EACCESS
#define EACCESS (EACCES)
#endif // !EACCESS
#ifndef ENETRESET
#define ENETRESET (NN_HAUSNUMERO + 18)
#endif // !ENETRESET
#ifndef ENETUNREACH
#define ENETUNREACH (NN_HAUSNUMERO + 19)
#endif // !ENETUNREACH
#ifndef EHOSTUNREACH
#define EHOSTUNREACH (NN_HAUSNUMERO + 20)
#endif // !EHOSTUNREACH
#ifndef ENOTCONN
#define ENOTCONN (NN_HAUSNUMERO + 21)
#endif // !ENOTCONN
#ifndef EMSGSIZE
#define EMSGSIZE (NN_HAUSNUMERO + 22)
#endif // !EMSGSIZE
#ifndef EETIMEDOUT
#define EETIMEDOUT (NN_HAUSNUMERO + 23)
#endif // !ETIMEDOUT
#ifndef ECONNABORTED
#define ECONNABORTED (NN_HAUSNUMERO + 24)
#endif // !ECONNABORTED
#ifndef ECONNRESET
#define ECONNRESET (NN_HAUSNUMERO + 25)
#endif // !ECONNRESET
#ifndef ENOPROTOOPT
#define ENOPROTOOPT (NN_HAUSNUMERO + 26)
#endif // !ENOPROTOOPT
#ifndef EISCONN
#define EISCONN (NN_HAUSNUMERO + 27)
#endif // !EISCONN
#ifndef ESOCKTNOSUPPORT
#define ESOCKTNOSUPPORT (NN_HAUSNUMERO + 28)
#endif // !ESOCKTNOSUPPORT

/* Native nanomsg error codes */
#ifndef ETERM
#define ETERM (NN_HAUSNUMERO + 53)
#endif // !ETERM
#ifndef EFSM
#define EFSM (NN_HAUSNUMERO + 54)
#endif // !EFSM

NN_EXPORT int nn_errno(void);

NN_EXPORT const char* nn_strerror(int errnum);

NN_EXPORT const char* nn_symbol(int i, int* value);

/* nn_symbol_properties 中的 ns 常量 */
#define NN_NS_NAMESPACE 0
#define NN_NS_VERSION 1
#define NN_NS_DOMAIN 2
#define NN_NS_TRANSPORT 3
#define NN_NS_PROTOCOL 4
#define NN_NS_OPTION_LEVEL 5
#define NN_NS_SOCKET_OPTION 6
#define NN_NS_TRANSPORT_OPTION 7
#define NN_NS_OPTION_TYPE 8
#define NN_NS_OPTION_UNIT 9
#define NN_NS_FLAG 10
#define NN_NS_ERROR 11
#define NN_NS_LIMIT 12
#define NN_NS_EVENT 13
#define NN_NS_STATISTIC 14

/* nn_symbol_properties 中的 type 常量 */
#define NN_TYPE_NONE 0
#define NN_TYPE_INT 1
#define NN_TYPE_STR 2

/* nn_symbol_properties 中的 unit 常量 */
#define NN_UNIT_NONE 0
#define NN_UNIT_BYTES 1
#define NN_UNIT_MILLISECONDS 2
#define NN_UNIT_PRIORITY 3
#define NN_UNIT_BOOLEAN 4
#define NN_UNIT_MESSAGES 5
#define NN_UNIT_COUNTER 6

/* nn_symbol返回的结构体类型 */
struct nn_symbol_properties {
    int value;
    const char* name;
    int ns;
    int type;
    int unit;
};

NN_EXPORT int nn_symbol_info(int i, struct nn_symbol_properties* buf, int buflen);

NN_EXPORT void nn_term(void);

#define NN_MSG ((size_t)-1)

NN_EXPORT void* nn_allocmsg(size_t size, int type);
NN_EXPORT void* nn_reallocmsg(void* msg, size_t size);
NN_EXPORT void nn_freemsg(void* msg);

/* socket定义 */
struct nn_iovec {
    void* iov_base;
    size_t iov_len;
};

struct nn_msghdr {
    size_t cmsg_len;
    int cmsg_level;
    int cmsg_type;
};

NN_EXPORT struct nn_cmsghdr* nn_cmsg_nxthdr_(const struct nn_msghdr* mhdr, const struct nn_cmsghdr* cmsg);
#define NN_CMSG_ALIGN_(len) \
    (((len) + sizeof(size_t) - 1) & (size_t) ~(sizeof(size_t) - 1))

#define NN_CMSG_FIRSTHDR(mhdr) \
    nn_cmsg_nxthdr_((struct nn_msghdr*)(mhdr), NULL)

#define NN_CMSG_NXTHDR(mhdr, cmsg) \
    nn_cmsg_nxthdr_((struct nn_msghdr*)(mhdr), (struct nn_cmsghdr*)(cmsg))

#define NN_CMSG_DATA(cmsg) \
    ((unsigned char*)(((struct nn_cmsghdr*)(cmsg)) + 1))

#define NN_CMSG_SPACE(len) \
    (NN_CMSG_ALIGN_(len) + NN_CMSG_ALIGN_(sizeof(struct nn_cmsghdr)))

#define NN_CMSG_LEN(len) \
    (NN_CMSG_ALIGN_(sizeof(struct nn_cmsghdr)) + (len))

/* SP地址族 */
#define AF_SP 1
#define AF_SP_RAW 2

#define NN_SOCKADDR_MAX 128

#define NN_SOL_SOCKET 0

/* 通用socket选项 */
#define NN_LINGER 1
#define NN_SNDBUF 2
#define NN_RCVBUF 3
#define NN_SNDTIMEO 4
#define NN_RCVTIMEO 5
#define NN_RECONNECT_IVL 6
#define NN_RECONNECT_IVL_MAX 7
#define NN_SNDPRIO 8
#define NN_RCVPRIO 9
#define NN_SNDFD 10
#define NN_RCVFD 11
#define NN_DOMAIN 12
#define NN_PROTOCOL 13
#define NN_IPV4ONLY 14
#define NN_SOCKET_NAME 15
#define NN_RCVMAXSIZE 16
#define NN_MAXTTL 17

#define NN_DONTWAIT 1

#define PROTO_SP 1
#define SP_HDR 1

NN_EXPORT int nn_socket(int domain, int protocol);
NN_EXPORT int nn_close(int s);
NN_EXPORT int nn_setsocketopt(int s, int level, int option, const void* optval,
    size_t optvallen);
NN_EXPORT int nn_getsocketopt(int s, int level, int option, void* optval,
    size_t* optvallen);
NN_EXPORT int nn_bind(int s, const void* addr);
NN_EXPORT int nn_connect(int s, const void* addr);
NN_EXPORT int nn_shutdown(int s, int how);
NN_EXPORT int nn_send(int s, const void* buf, size_t len, int flags);
NN_EXPORT int nn_recv(int s, void* buf, size_t len, int flags);
NN_EXPORT int nn_sendmsg(int s, const struct nn_msghdr* msghdr, int flags);
NN_EXPORT int nn_recvmsg(int s, struct nn_msghdr* msghdr, int flags);

#define NN_POLLIN 1
#define NN_POLLOUT 2

struct nn_pollfd {
    int fd;
    short events;
    short revents;
};

NN_EXPORT int nn_poll(struct nn_pollfd* fds, int nfds, int timeout);

NN_EXPORT int nn_device(int s1, int s2);

#define NN_STAT_ESTABLISHED_CONNECTIONS 101
#define NN_STAT_ACCEPTED_CONNECTIONS 102
#define NN_STAT_DROPPED_CONNECTIONS 103
#define NN_STAT_BROKEN_CONNECTIONS 104
#define NN_STAT_CONNECT_ERRORS 105
#define NN_STAT_BIND_ERRORS 106
#define NN_STAT_ACCEPT_ERRORS 107

#define NN_STAT_CURRENT_CONNECTIONS 201
#define NN_STAT_INPROGRESS_CONNECTIONS 202
#define NN_STAT_CURRENT_EP_ERRORS 203

#define NN_STAT_MESSAGES_SENT 301
#define NN_STAT_MESSAGES_RECEIVED 302
#define NN_STAT_BYTES_SENT 303
#define NN_STAT_BYTES_RECEIVED 304

#define NN_STAT_CURRENT_SND_PRIORITY 401

NN_EXPORT uint64_t nn_get_statistic(int s, int stat);

#ifdef __cplusplus
}
#endif

#endif // !NN_H_INCLUDED
