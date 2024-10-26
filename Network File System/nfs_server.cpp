#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>

struct FileMetadata {
    std::string filename;
    size_t size;
    time_t last_modified;
    std::string permissions;
};

class NFSServer {
private:
    int server_fd;
    int port;
    std::string base_directory;
    std::map<std::string, FileMetadata> file_table;

public:
    NFSServer(int port = 8080, std::string base_dir = "./nfs_root/") 
        : port(port), base_directory(base_dir) {
        // Create base directory if it doesn't exist
        mkdir(base_directory.c_str(), 0777);
    }

    void start() {
        struct sockaddr_in address;
        int opt = 1;

        // Create socket
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == 0) {
            throw std::runtime_error("Socket creation failed");
        }

        // Set socket options - only use SO_REUSEADDR for macOS
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
            throw std::runtime_error("Setsockopt failed");
        }

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        // Bind socket
        if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
            throw std::runtime_error("Bind failed");
        }

        // Listen for connections
        if (listen(server_fd, 3) < 0) {
            throw std::runtime_error("Listen failed");
        }

        std::cout << "NFS Server started on port " << port << std::endl;
        handle_connections();
    }

private:
    void handle_connections() {
        while (true) {
            struct sockaddr_in client_addr;
            socklen_t addrlen = sizeof(client_addr);
            
            int client_socket = accept(server_fd, (struct sockaddr*)&client_addr, &addrlen);
            if (client_socket < 0) {
                std::cerr << "Accept failed" << std::endl;
                continue;
            }

            char client_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
            std::cout << "New connection from " << client_ip << std::endl;

            handle_client(client_socket);
        }
    }

    void handle_client(int client_socket) {
        char buffer[1024] = {0};
        ssize_t bytes_read = read(client_socket, buffer, 1024);
        
        if (bytes_read > 0) {
            std::string request(buffer);
            std::string response = process_request(request);
            send(client_socket, response.c_str(), response.length(), 0);
        }
        
        close(client_socket);
    }

    std::string process_request(const std::string& request) {
        std::istringstream iss(request);
        std::string operation;
        iss >> operation;

        std::cout << "Processing request: " << operation << std::endl;

        if (operation == "READ") {
            std::string filename;
            iss >> filename;
            return read_file(filename);
        }
        else if (operation == "WRITE") {
            std::string filename, content;
            iss >> filename;
            std::getline(iss, content);
            return write_file(filename, content.substr(1)); // Remove leading space
        }
        else if (operation == "LIST") {
            return list_files();
        }
        else if (operation == "DELETE") {
            std::string filename;
            iss >> filename;
            return delete_file(filename);
        }
        
        return "Invalid operation";
    }

    std::string read_file(const std::string& filename) {
        std::string filepath = base_directory + filename;
        std::ifstream file(filepath);
        
        if (!file.is_open()) {
            return "Error: File not found";
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    std::string write_file(const std::string& filename, const std::string& content) {
        std::string filepath = base_directory + filename;
        std::ofstream file(filepath);
        
        if (!file.is_open()) {
            return "Error: Cannot write file";
        }

        file << content;
        update_file_table(filename);
        return "File written successfully";
    }

    std::string list_files() {
        std::string result;
        DIR* dir = opendir(base_directory.c_str());
        if (dir != nullptr) {
            struct dirent* entry;
            while ((entry = readdir(dir)) != nullptr) {
                if (entry->d_name[0] != '.') {  // Skip hidden files
                    result += std::string(entry->d_name) + "\n";
                }
            }
            closedir(dir);
        }
        return result.empty() ? "No files found" : result;
    }

    std::string delete_file(const std::string& filename) {
        std::string filepath = base_directory + filename;
        if (remove(filepath.c_str()) != 0) {
            return "Error: Cannot delete file";
        }
        file_table.erase(filename);
        return "File deleted successfully";
    }

    void update_file_table(const std::string& filename) {
        FileMetadata metadata;
        metadata.filename = filename;
        
        std::string filepath = base_directory + filename;
        struct stat file_stat;
        if (stat(filepath.c_str(), &file_stat) == 0) {
            metadata.size = file_stat.st_size;
            metadata.last_modified = file_stat.st_mtime;
            metadata.permissions = std::to_string(file_stat.st_mode & 0777);
        }
        
        file_table[filename] = metadata;
    }
};

int main() {
    try {
        std::cout << "Starting NFS Server..." << std::endl;
        NFSServer server(8080, "./nfs_root/");
        server.start();
    } catch (const std::exception& e) {
        std::cerr << "Server Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}