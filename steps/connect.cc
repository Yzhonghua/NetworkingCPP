/*
 * ************ SOCKET *************
 * int socket(int domain, int type, int protocol);
 * It doesn't actually connect to any thing on theinternet.
 * It just set up the data structure in the operating system for the connection.
 * More specifically, it finds a slot in the file descripter table,
 *  allocates a slot to it, and tag it as a socket (internet stuff using this protocol).
 * ************ CONNECT *************
 * int connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen);
 * Op will send message to the remote machine: "hi, we want to connect".
 */
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>

void Usage(char *progname);

bool LookupName(char *name,
                unsigned short port,
                struct sockaddr_storage *ret_addr,
                size_t *ret_addrlen);

int main(int argc, char **argv) {
    if (argc != 3) {
        Usage(argv[0]);
    }

    unsigned short port = 0;
    if (sscanf(argv[2], "%hu", &port) != 1) {
        Usage(argv[0]);
    }

    // get an appropriate sockaddr structure.
    struct sockaddr_storage addr;
    size_t addrlen;
    if (!LookupName(argv[1], port, &addr, &addrlen)) {
        Usage(argv[0]);
    }

    // create the socket.
    int socket_fd = socket(addr.ss_family, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        std::cerr << "socket() failed: " << strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }

    int res = connect(socket_fd,
                      reinterpret_cast<sockaddr *>(&addr),
                      addrlen);
    if (res == -1) {
        std::cerr << "connect() failed: " << strerror(errno) << std::endl;
    }

    // clean up.
    close(socket_fd);
    return EXIT_SUCCESS;
}

void Usage(char *progname) {
    std::cerr << "Usage: " << progname << " hostname port" << std::endl;
    exit(EXIT_FAILURE);
}

bool LookupName(char *name,
                unsigned short port,
                struct sockaddr_storage *ret_addr,
                size_t *ret_addrlen) {
    struct addrinfo hints, *results;
    int retval;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // do the lookup by invoking getaddrinfo().
    if ((retval = getaddrinfo(name, nullptr, &hints, &results)) != 0) {
        std::cerr << "getaddrinfo failed: ";
        std::cerr << gai_strerror(retval) << std::endl;
        return EXIT_FAILURE;
    }

    // set the port in the first result.
    if (results->ai_family == AF_INET) {
        struct sockaddr_in *v4addr = (struct sockaddr_in *) results->ai_addr;
        v4addr->sin_port = htons(port);
    } else if (results->ai_family == AF_INET6) {
        struct sockaddr_in6 *v6addr = (struct sockaddr_in6 *) results->ai_addr;
        v6addr->sin6_port = htons(port);
    } else {
        std::cerr << "getaddrinfo failed to provide an IPv4 or IPv6 address";
        std::cerr << std::endl;
        freeaddrinfo(results);
        return false;
    }

    // return the first result.
    assert(results != nullptr);
    memcpy(ret_addr, results->ai_addr, results->ai_addrlen);
    *ret_addrlen = results->ai_addrlen;

    // clean up
    freeaddrinfo(results);
    return true;
}
