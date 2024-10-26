#pragma once
#include <string>
#include <functional>
#include <unordered_map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

class RPCServer {
private:
    int serverSocket;
    int port;
    std::unordered_map<std::string, std::function<std::string(std::string)>> procedures;

public:
    RPCServer(int port) : port(port) {}

    void registerProcedure(const std::string& name, 
                          std::function<std::string(std::string)> procedure) {
        procedures[name] = procedure;
    }

    void start();

private:
    void handleClient(int clientSocket);
};
