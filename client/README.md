### Socket API: Client TCP Connection

There are five steps:

* Figure out the IP address and port to connect to
* Create a socket
* connect the socket to the remote server
* read( ) and write( ) data using the socket
* Close the socket

### Usage:

server: nc -l port output.bytes

client: ./client host port demo.txt
