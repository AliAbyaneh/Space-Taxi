all: client

client: main.cpp client_socket.o tools.o
	g++ main.cpp client_socket.o tools.o -o client
	
client_socket.o: client_socket.cpp client_socket.h tools.h network_exception.h
	g++ -c client_socket.cpp
	
tools.o: tools.cpp tools.h
	g++ -c tools.cpp
	
clean:
	rm *.o client
