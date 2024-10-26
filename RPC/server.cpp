#include "server.h"

void RPCServer::start() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        throw std::runtime_error("Failed to create socket");
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        throw std::runtime_error("Failed to bind socket");
    }

    listen(serverSocket, 5);
    std::cout << "Server listening on port " << port << std::endl;

    while (true) {
        sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);

        if (clientSocket < 0) {
            std::cerr << "Failed to accept connection" << std::endl;
            continue;
        }

        handleClient(clientSocket);
        close(clientSocket);
    }
}

void RPCServer::handleClient(int clientSocket) {
    char buffer[1024] = {0};
    read(clientSocket, buffer, 1024);

    std::string request(buffer);
    size_t delimPos = request.find('|');
    if (delimPos == std::string::npos) {
        std::string response = "Invalid request format";
        send(clientSocket, response.c_str(), response.length(), 0);
        return;
    }

    std::string procName = request.substr(0, delimPos);
    std::string args = request.substr(delimPos + 1);

    if (procedures.find(procName) == procedures.end()) {
        std::string response = "Procedure not found";
        send(clientSocket, response.c_str(), response.length(), 0);
        return;
    }

    std::string result = procedures[procName](args);
    send(clientSocket, result.c_str(), result.length(), 0);
}