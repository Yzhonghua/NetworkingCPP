#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

void Usage(char *progname);
void PrintOut(int fd, struct sockaddr *addr, size_t addrlen);

int main(int argc, char **argv) {
    // expect the port number as a command line argument
    if (argc != 2) {
        Usage(argv[0]);
    }

    // populate the "hints" addrinfo structure for getaddrinfo().
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET6;       // IPv6 (also handles IPv4 clients)
                                      // If we do AF_UNSPEC like clinet side
                                      // It may be set as IPv4 which can't handle IPv6
    hints.ai_socktype = SOCK_STREAM;  // stream
    hints.ai_flags = AI_PASSIVE;      // use wildcard "in6addr_any" address
    hints.ai_flags |= AI_V4MAPPED;    // use v4-mapped v6 if no v6 found
    hints.ai_protocol = IPPROTO_TCP;  // tcp protocol
    hints.ai_canonname = nullptr;
    hints.ai_addr = nullptr;
    hints.ai_next = nullptr;

    // use argv[1] as the string representation of out portnumber to
    // pass into getaddrinfo(). getaddrinfo() returns a list of
    // address structures via the output parameter "result".
    // I use nullptr as the first argument, which represents wildcard address,
    // i.e. the socket will be able to accept connections directed to any of the machine's IP addresses
    struct addrinfo *result;
    int res = getaddrinfo(nullptr, argv[1], &hints, &result);

    // Did addrinfo() fail?
    if (res != 0) {
        std::cerr << "getaddrinfo() failed: ";
        std::cerr << gai_strerror(res) << std::endl;
        return EXIT_FAILURE;
    }

    // Loop through the returned address structures until we are able
    // to create a socket and bind to one. The address structures are
    // linked in a list through the "ai_next" field of result.
    int listen_fd = -1;
    for (struct addrinfo *rp = result; rp != nullptr; rp = rp->ai_next) {
        listen_fd = socket(rp->ai_family,
                           rp->ai_socktype,
                           rp->ai_protocol);
        if (listen_fd == -1) {
            // creating this socket failed. So, loop to the next returned
            // result and try again.
            std::cerr << "socket() failed: " << strerror(errno) << std::endl;
            // listen_fd = 0;
            continue;
        }

        // Confugure the socket; we're setting a socket "option". In
        // particular, we set "SO_REUSEADDR", which tells the TCP stack
        // so make the port we bind to avaliable again as soon as we
        // exit, rather than waiting for a few tens of seconds to recycle it.
        int optval = 1;
        setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR,
                   &optval, sizeof(optval));
            
        // Try binding the socket to the address and port number returned
        // by getaddrinfo().
        if (bind(listen_fd, rp->ai_addr, rp->ai_addrlen) == 0) {
            // Bind worked! Print out the information about what
            // we bound to.
            PrintOut(listen_fd, rp->ai_addr, rp->ai_addrlen);
            break;
        }

        // The bind failed. Close the socket, then loop back around and
        // try the next address/port returned by getaddrinfo().
        close(listen_fd);
        listen_fd = -1;
    }

    // Free the structure returned by getaddrinfo().
    freeaddrinfo(result);

    // Did we succeed in binding to any address?
    if (listen_fd == -1) {
        // No. quit with failure.
        std::cerr << "Couldn't bind to any addresses." << std::endl;
        return EXIT_FAILURE;
    }

    // Success. Tell the OS that we want this to be a listening socket.
    if (listen(listen_fd, SOMAXCONN) != 0) {
        std::cerr << "Failed to mark socket as listening: ";
        std::cerr << strerror(errno) << std::endl;
        close(listen_fd);
        return EXIT_FAILURE;
    }

    // Sleep for 20 seconds, then close house.
    sleep(20);
    close(listen_fd);
    return EXIT_FAILURE;
}

void Usage(char *progname) {
    std::cerr << "Usage: " << progname << " port" << std::endl;
    exit(EXIT_FAILURE);
}

void PrintOut(int fd, struct sockaddr *addr, size_t addrlen) {
  std::cout << "Socket [" << fd << "] is bound to:" << std::endl;
  if (addr->sa_family == AF_INET) {
    // Print out the IPV4 address and port

    char astring[INET_ADDRSTRLEN];
    struct sockaddr_in *in4 = reinterpret_cast<struct sockaddr_in *>(addr);
    inet_ntop(AF_INET, &(in4->sin_addr), astring, INET_ADDRSTRLEN);
    std::cout << " IPv4 address " << astring;
    std::cout << " and port " << ntohs(in4->sin_port) << std::endl;

  } else if (addr->sa_family == AF_INET6) {
    // Print out the IPV6 address and port

    char astring[INET6_ADDRSTRLEN];
    struct sockaddr_in6 *in6 = reinterpret_cast<struct sockaddr_in6 *>(addr);
    inet_ntop(AF_INET6, &(in6->sin6_addr), astring, INET6_ADDRSTRLEN);
    std::cout << " IPv6 address " << astring;
    std::cout << " and port " << ntohs(in6->sin6_port) << std::endl;

  } else {
    std::cout << " ???? address and port ????" << std::endl;
  }
}
