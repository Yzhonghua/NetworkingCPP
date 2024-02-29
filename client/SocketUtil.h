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

#endif  // _SOCKETUTIL_H_