#ifndef INET_SOCKET_H
#define INET_SOCKET_H

#include <sys/socket.h>
#include <netdb.h>

/* Creates a socket with the given socket type, and connects it to
 * the address specified by host and service */
int inetConnect(const char *host, const char *service, int type);

/* Creates a listening stream(SOCK_STREAM) socket bound to the
 * wildcared IP address on the TCP port specified by service */
int inetListen(const char *service, int backlog, socklen_t *addrlen);

/* Creates a socket of the given type, bound to the wildcard IP address
 * on the port specified by service and type */
int inetBind(const char *service, int type, socklen_t *addrlen);

char *inetAddressStr(const struct sockaddr *addr, socklen_t addrlen,
			char *addrStr, int addrStrLen);

#define IS_ADDR_STR_LEN 4096
			/* Suggested length for string buffer that caller
			 * should pass to inetAddressStr(). Must be greater
			 * than (NI_MAXHOST + NI_MAXSERV + 4) */

#endif
