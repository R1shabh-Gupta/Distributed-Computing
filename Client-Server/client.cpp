#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

class TCPClient {
private:
    int sock;
    struct sockaddr_in serv_addr;
    const int PORT = 8080;
    const char* SERVER_IP = "127.0.0.1";

public:
    TCPClient() {
        // Create socket
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            throw std::runtime_error("Socket creation failed");
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);

        // Convert IP address to binary form
        if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
            throw std::runtime_error("Invalid address");
        }
    }

    void connect_to_server() {
        if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
            throw std::runtime_error("Connection failed");
        }
    }

    void send_message(const std::string& message) {
        send(sock, message.c_str(), message.length(), 0);
        
        char buffer[1024] = {0};
        int valread = read(sock, buffer, 1024);
        std::cout << "Server response: " << buffer << std::endl;
    }

    ~TCPClient() {
        close(sock);
    }
};

int main() {
    try {
        TCPClient client;
        client.connect_to_server();
        client.send_message("Hello from client!");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}