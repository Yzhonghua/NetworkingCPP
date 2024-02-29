CXX = g++
CPPFLAGS = -Wall -g -std=c++17

server: netcopyserver.o SocketUtil.o
	$(CXX) $(CPPFLAGS) -o ex16 netcopyserver.o SocketUtil.o

netcopyserver.o: netcopyserver.cc
	$(CXX) $(CPPFLAGS) -c netcopyserver.cc

SocketUtil.o: SocketUtil.cc
	$(CXX) $(CPPFLAGS) -c SocketUtil.cc

clean:
	/bin/rm -f -r *~ *.o server server.dSYM