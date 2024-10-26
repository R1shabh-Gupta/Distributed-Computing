#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

class TCPServer {
private:
    int server_fd, client_socket;
    struct sockaddr_in address;
    const int PORT = 8080;

public:
    TCPServer() {
        // Create socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            throw std::runtime_error("Socket creation failed");
        }
        // Set socket options
        int opt = 1;
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
            throw std::runtime_error("Setsockopt failed");
        }

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);

        // Bind socket to address
        if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
            throw std::runtime_error("Bind failed");
        }
    }
    void start() {
        // Listen for connections
        if (listen(server_fd, 3) < 0) {
            throw std::runtime_error("Listen failed");
        }

        std::cout << "Server listening on port " << PORT << std::endl;

        while (true) {
            int addrlen = sizeof(address);
            if ((client_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
                throw std::runtime_error("Accept failed");
            }

            handle_client();
        }
    }
    void handle_client() {
        char buffer[1024] = {0};
        int valread = read(client_socket, buffer, 1024);
        std::cout << "Message from client: " << buffer << std::endl;

        std::string response = "Message received by server";
        send(client_socket, response.c_str(), response.length(), 0);
        close(client_socket);
    }

    ~TCPServer() {
        close(server_fd);
    }
};
int main() {
    try {
        TCPServer server;
        server.start();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}