#ifndef _SOCKETUTIL_H_
#define _SOCKETUTIL_H_

#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>

// Do a DNS lookup on name "name", and return through the output
// parameter "ret_addr" a fully-formed struct sockaddr.  The length
// of that struct sockaddr is returned through "ret_addrlen".  Also
// initialize the port to "port".
//
// On failure, returns false.  On success, returns true.
bool LookupName(char *name,
                unsigned short port,
                struct sockaddr_storage *ret_addr,
                size_t *ret_addrlen);

// Connect to the remote host and port specified by addr/addrlen.
// Return a connected socket through the output parameter "ret_fd".
//
// On failure, returns false.  On success, returns true.
bool Connect(const struct sockaddr_storage &addr,
             const size_t &addrlen,
             int *ret_fd);

// Create a listening socket that is bound to any local IP
// address and to the port number specified by "portnum."
//
// On success, return a file descriptor for the created listening
// socket.  The caller is responsible for eventually close()'ing
// the socket.  Also return through the output parameter
// "sock_family" the network address family (i.e., AF_INET or
// AF_INET6).
//
// On failure, returns -1.
int Listen(char *portnum, int *sock_family);

// A wrapper around "read" that shields the caller from dealing
// with the ugly issues of partial reads, EINTR, EAGAIN, and so
// on.
//
// Read at most "readlen" bytes from the file descriptor fd
// into the buffer "buf".  Return the number of bytes actually
// read.  On fatal error, return -1.  If EOF is hit and no
// bytes have been read, return 0.  Might read fewer bytes
// than requested.
int WrappedRead(int fd, unsigned char *buf, int readlen);

// A wrapper around "write" that shields the caller from dealing
// with the ugly issues of partial writes, EINTR, EAGAIN, and so
// on.
//
// Write "writelen" bytes to the file descriptor fd from
// the buffer "buf".  Blocks the caller until either writelen
// bytes have been written, or an error is encountered.  Return
// the total number of bytes written; if this number is less
// than writelen, it's because some fatal error was encountered,
// like the connection being dropped.
int WrappedWrite(int fd, unsigned char *buf, int writelen);

// Print to stdout information about the file descriptor "fd" and
// the struct sockaddr pointer to by "addr".  In particular,
// print out the IPv4/IPv6 address and port number.
void PrintOut(int fd, struct sockaddr *addr, size_t addrlen);

// Attempt to do a reverse DNS lookup on to find the DNS name
// associated with the IPv4/IPv6 address inside "addr."  Will
// either print to stdout the DNS name or an error message if the
// reverse DNS lookup fails.
void PrintReverseDNS(struct sockaddr *addr, size_t addrlen);

#endif  // _SOCKETUTIL_H_