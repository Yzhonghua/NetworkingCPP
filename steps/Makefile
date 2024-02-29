CXX = g++
CPPFLAGS = -Wall -g -std=c++17

dnsresolve: dnsresolve.cc
	$(CXX) $(CPPFLAGS) -o dnsresolve dnsresolve.cc

connect: connect.cc
	$(CXX) $(CPPFLAGS) -o connect connect.cc

sendreceive: sendreceive.cc
	$(CXX) $(CPPFLAGS) -o sendreceive sendreceive.cc

server_bind_listen: server_bind_listen.cc
	$(CXX) $(CPPFLAGS) -o server_bind_listen server_bind_listen.cc

server_accept_rw_close: server_accept_rw_close.cc
	$(CXX) $(APPFLAGS) -o server_accept_rw_close server_accept_rw_close.cc

clean:
	/bin/rm -f -r *~ *.o dnsresolve connect sendreceive server_bind_listen server_accept_rw_close *.dSYM