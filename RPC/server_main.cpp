#include "server.h"
#include <iostream>

int main() {
    RPCServer server(8080);
    
    // Register a procedure that adds two numbers
    server.registerProcedure("add", [](const std::string& args) -> std::string {
        size_t pos = args.find(',');
        if (pos == std::string::npos) return "Invalid arguments";
        
        try {
            int a = std::stoi(args.substr(0, pos));
            int b = std::stoi(args.substr(pos + 1));
            return std::to_string(a + b);
        } catch (const std::exception& e) {
            return "Invalid number format";
        }
    });

    // Register a procedure that concatenates strings
    server.registerProcedure("concat", [](const std::string& args) -> std::string {
        size_t pos = args.find(',');
        if (pos == std::string::npos) return "Invalid arguments";
        
        std::string str1 = args.substr(0, pos);
        std::string str2 = args.substr(pos + 1);
        return str1 + str2;
    });

    try {
        server.start();
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}