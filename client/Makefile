CXX = g++
CPPFLAGS = -Wall -g -std=c++17

client: netcopy.o SocketUtil.o
	$(CXX) $(CPPFLAGS) -o ex15 netcopy.o SocketUtil.o

netcopy.o: netcopy.cc
	$(CXX) $(CPPFLAGS) -c netcopy.cc

SocketUtil.o: SocketUtil.cc
	$(CXX) $(CPPFLAGS) -c SocketUtil.cc

clean:
	/bin/rm -f -r *~ *.o client client.dSYM