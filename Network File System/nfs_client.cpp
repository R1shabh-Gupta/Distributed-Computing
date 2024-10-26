#include <iostream>
#include <string>
#include <cstring>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

class NFSClient {
private:
    std::string server_ip;
    int port;

public:
    NFSClient(std::string ip = "127.0.0.1", int port = 8080) 
        : server_ip(ip), port(port) {}

    std::string send_request(const std::string& request) {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            return "Error: Socket creation failed";
        }

        struct sockaddr_in serv_addr;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);

        if (inet_pton(AF_INET, server_ip.c_str(), &serv_addr.sin_addr) <= 0) {
            return "Error: Invalid address";
        }

        if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
            return "Error: Connection failed";
        }

        send(sock, request.c_str(), request.length(), 0);
        
        char buffer[1024] = {0};
        read(sock, buffer, 1024);
        
        close(sock);
        return std::string(buffer);
    }

    std::string read_file(const std::string& filename) {
        return send_request("READ " + filename);
    }

    std::string write_file(const std::string& filename, const std::string& content) {
        return send_request("WRITE " + filename + " " + content);
    }

    std::string list_files() {
        return send_request("LIST");
    }

    std::string delete_file(const std::string& filename) {
        return send_request("DELETE " + filename);
    }
};

int main() {
    try {
        NFSClient client("127.0.0.1", 8080);
        std::string command, filename, content;
        
        std::cout << "NFS Client started. Available commands:" << std::endl;
        std::cout << "1. READ <filename>" << std::endl;
        std::cout << "2. WRITE <filename> <content>" << std::endl;
        std::cout << "3. LIST" << std::endl;
        std::cout << "4. DELETE <filename>" << std::endl;
        std::cout << "5. EXIT" << std::endl;

        while (true) {
            std::cout << "\nEnter command: ";
            std::getline(std::cin, command);

            if (command == "EXIT") {
                break;
            }
            else if (command == "LIST") {
                std::cout << "Files in server:" << std::endl;
                std::cout << client.list_files() << std::endl;
            }
            else {
                std::istringstream iss(command);
                std::string operation;
                iss >> operation;

                if (operation == "READ") {
                    iss >> filename;
                    std::cout << "File content:" << std::endl;
                    std::cout << client.read_file(filename) << std::endl;
                }
                else if (operation == "WRITE") {
                    iss >> filename;
                    std::getline(iss, content);
                    if (content.empty() || content == " ") {
                        std::cout << "Enter content: ";
                        std::getline(std::cin, content);
                    }
                    std::cout << client.write_file(filename, content) << std::endl;
                }
                else if (operation == "DELETE") {
                    iss >> filename;
                    std::cout << client.delete_file(filename) << std::endl;
                }
                else {
                    std::cout << "Invalid command!" << std::endl;
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Client Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}