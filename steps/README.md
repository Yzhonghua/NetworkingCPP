This folder contains several features that make up the client and server, which can also be used separately.

### Usage:

**dnsresolve.cc**

./dnsresolve.cc hostname

this will print all IPv6 and IPv6 addresses refer to the hostname.

**connect.cc.**

server: nc -l port

client: ./connect hostname port

this will make connection and then shut down, with nothing printed.

**sendreceive.cc**

server: nc -l port

client: ./sendreceive hostname port

this will make connection, allowing server to send a message and client will send it back.

**server_bind_listen.cc**

server: ./server_bind_listen port

client: nc hostname port

this will allow server to listen on wildcard address: port, and let client to make connection. Once they connected, server will wait 20s and then shut down.

**server_accept_rw_close.cc**

server: ./server_accept_rw_close port

client: nc hostname port

this will make connection, allowing client to send message to server, which will both shows in client and server side.

**Additional**

![1709237598776](image/README/1709237598776.png)

when we use ./server_function port, we set nullptr to getaddrinfo( ) as thr first parameter to indicate that we allow wildcard address here. So we can deifnitely use 127.0.0.1 or localhost or hostname to get connection, but can also use dnsresolve we have to get more if we want.
