#include "client.h"
#include <iostream>

int main() {
    RPCClient client("127.0.0.1", 8080);

    try {
        // Call the add procedure
        std::string result1 = client.call("add", "5,3");
        std::cout << "5 + 3 = " << result1 << std::endl;

        // Call the concat procedure
        std::string result2 = client.call("concat", "Hello,World");
        std::cout << "Concatenated string: " << result2 << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Client error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}