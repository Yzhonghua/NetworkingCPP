#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>

void Usage(char *progname) {
    std::cerr << "Usage:" << progname << " hostname" << std::endl;
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    int retval;
    struct addrinfo hints, *results, *r;

    if (argc != 2) {
        Usage(argv[0]);
    }

    // zero out the hints data structure using memset.
    memset(&hints, 0, sizeof(hints));

    // indicate we're happy with bith AF_INET or AF_INET6 addresses.
    hints.ai_family = AF_UNSPEC;

    // constrain the answer to SOCK_STREAM addresses.
    hints.ai_socktype = SOCK_STREAM;

    // do the lookup by invoking getaddrinfo().
    if ((retval = getaddrinfo(argv[1], nullptr, &hints, &results)) != 0) {
        std::cerr << "getaddrinfo failed: ";
        // gai_strerror is defined in netdb.h and return a
        // string describing getaddrinfo() error value.
        std::cerr << gai_strerror(retval) << std::endl;
        return EXIT_FAILURE;
    }

    // print out the result!
    std::cout << "Here are the IP addresses found for '" << argv[1];
    std::cout << "'" << std::endl;
    for (r = results; r != nullptr; r = r->ai_next) {
        // treat the IPv4 and IPv6 cases differently.
        if (r->ai_family == AF_INET) {
            char ipstring[INET_ADDRSTRLEN];
            struct sockaddr_in *v4addr = (struct sockaddr_in *) r->ai_addr;
            inet_ntop(r->ai_family,
                      &(v4addr->sin_addr),
                      ipstring,
                      INET_ADDRSTRLEN);
            std::cout << "  IPv4: " << ipstring << std::endl;
        } else if (r->ai_family == AF_INET6) {
            char ipstring[INET6_ADDRSTRLEN];
            struct sockaddr_in6 *v6addr = (struct sockaddr_in6 *) r->ai_addr;
            inet_ntop(r->ai_family,
                      &(v6addr->sin6_addr),
                      ipstring,
                      INET6_ADDRSTRLEN);
            std::cout << "  IPv6: " << ipstring << std::endl;
        } else {
            std::cout << "  unknown address family " << r->ai_family << std::endl;
        }
    }
    // clean up
    freeaddrinfo(results);
    return EXIT_SUCCESS;
}
