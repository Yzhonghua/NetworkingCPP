#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include "./SocketUtil.h"

static constexpr int kBufSize = 1024;


// Listen and accept a connection on a socket, then
// read data from the socket and write it to stdout.

void Usage(char *progname);

int main(int argc, char **argv) {
  if (argc != 2) {
    Usage(argv[0]);
  }

  // Create the listening socket.
  int sock_family, listen_fd;
  listen_fd = Listen(argv[1], &sock_family);
  if (listen_fd == -1) {
    std::cerr << "Failed to created listening socket on port "
              << argv[1] << std::endl;
    Usage(argv[0]);
  }

  // Wait for a client to arrive.
  int client_fd;
  while (1) {
    struct sockaddr_storage caddr;
    socklen_t caddr_len = sizeof(caddr);
    client_fd = accept(listen_fd,
                           reinterpret_cast<struct sockaddr *>(&caddr),
                           &caddr_len);
    if (client_fd < 0) {
      if ((errno == EAGAIN) || (errno == EINTR))
        continue;
      std::cerr << "Failure on accept: " << strerror(errno) << std::endl;
      Usage(argv[0]);
    }
    break;
  }

  // Loop, reading data from client_fd and writing it to stdout.
  while (1) {
    unsigned char buf[kBufSize];
    int res = WrappedRead(client_fd, buf, kBufSize);
    if (res <= 0)
      break;

    int wres = fwrite(buf, 1, res, stdout);
    if (wres != res)
      break;
  }

  close(client_fd);
  close(listen_fd);
  return EXIT_SUCCESS;
}

void Usage(char *progname) {
  std::cerr << "usage: " << progname << " port" << std::endl;
  exit(EXIT_FAILURE);
}