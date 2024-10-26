#include "client.h"

std::string RPCClient::call(const std::string& procName, const std::string& args) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        throw std::runtime_error("Failed to create socket");
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr) <= 0) {
        throw std::runtime_error("Invalid address");
    }

    if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        throw std::runtime_error("Connection failed");
    }

    std::string request = procName + "|" + args;
    send(sock, request.c_str(), request.length(), 0);

    char buffer[1024] = {0};
    read(sock, buffer, 1024);

    close(sock);
    return std::string(buffer);
}