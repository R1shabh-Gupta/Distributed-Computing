#pragma once
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>

class RPCClient {
private:
    std::string serverIP;
    int port;

public:
    RPCClient(const std::string& ip, int port) : serverIP(ip), port(port) {}
    std::string call(const std::string& procName, const std::string& args);
};