### Socket API: Server TCP Connection

* Figure out the IP address and port on which to listen
* Create a socket
* bind( ) the socket to the address(es) and port
* Tell the socket to listen( ) for incoming clients
* accept( ) a client connection
* read( ) and write( ) to that connection
* close( ) the client socket

### Usage

server: ./server port > output.bytes

client: ./client host port demo.txt

verify: md5sum output.bytes == md5sum demo.txt
