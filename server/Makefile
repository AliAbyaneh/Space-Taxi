server: main.o server_socket.o server.o	tools.o SpaceTAXI.o  User.o  Recorder.o Driver_Recorder.o Trip.o Time.o Galaxy.o
	g++ main.o server_socket.o server.o tools.o -o server SpaceTAXI.o  User.o Recorder.o Driver_Recorder.o Trip.o Time.o Galaxy.o

main.o: main.cpp
	g++ -c main.cpp

server.o: server.cpp server.h
	g++ -c server.cpp
	
server_socket.o: server_socket.cpp server_socket.h tools.h network_exception.h
	g++ -c server_socket.cpp
	
tools.o: tools.cpp tools.h
	g++ -c tools.cpp
SpaceTAXI.o: SpaceTAXI.cpp SpaceTAXI.h
	g++ -c SpaceTAXI.cpp
User.o: User.cpp User.h
	g++ -c  User.cpp
Trip.o: Trip.cpp Trip.h
	g++ -c  Trip.cpp	
Recorder.o: Recorder.cpp Recorder.h
	g++ -c  Recorder.cpp
Driver_Recorder.o: Driver_Recorder.cpp Driver_Recorder.h
	g++ -c  Driver_Recorder.cpp
Time.o: Time.cpp Time.h
	g++ -c  Time.cpp
Galaxy.o: Galaxy.cpp
	g++ -c Galaxy.cpp