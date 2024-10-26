CXX = g++
CXXFLAGS = -std=c++11 -Wall

all: server client

server: server_main.cpp server.cpp
	$(CXX) $(CXXFLAGS) server_main.cpp server.cpp -o server

client: client_main.cpp client.cpp
	$(CXX) $(CXXFLAGS) client_main.cpp client.cpp -o client

clean:
	rm -f server client
