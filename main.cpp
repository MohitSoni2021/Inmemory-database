#include <iostream>
#include <limits>
#include "core.hpp"

int main() {
    Redis redis;

    while (true) {
        std::cout << "\n1. Set\n"
                  << "2. Get\n"
                  << "3. Delete\n"
                  << "4. Set TTL\n"
                  << "5. Get TTL\n"
                  << "6. List keys\n"
                  << "0. Exit\n"
                  << "Choose an option: ";

        int option;
        if (!(std::cin >> option)) {
            std::cout << "Invalid option. Exiting...\n";
            return 1;
        }

        if (option == 0) {
            break;
        }

        std::string key;
        switch (option) {
        case 1: {
            std::string value;
            std::cout << "Enter key: ";
            std::cin >> key;
            std::cout << "Enter value: ";
            std::cin >> value;
            redis.set(key, value);
            std::cout << "Value stored.\n";
            break;
        }
        case 2: {
            std::cout << "Enter key: ";
            std::cin >> key;
            const std::optional<std::string> value = redis.get(key);
            if (value) {
                std::cout << "Value: " << *value << '\n';
            } else {
                std::cout << "Key not found.\n";
            }
            break;
        }
        case 3:
            std::cout << "Enter key: ";
            std::cin >> key;
            std::cout << (redis.del(key) ? "Key deleted.\n" : "Key not found.\n");
            break;
        case 4: {
            int seconds;
            std::cout << "Enter key: ";
            std::cin >> key;
            std::cout << "Enter TTL in seconds: ";
            if (!(std::cin >> seconds) || seconds < 0) {
                std::cout << "TTL must be a non-negative integer.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
            std::cout << (redis.set_ttl(key, seconds)
                              ? "TTL set.\n"
                              : "Key not found.\n");
            break;
        }
        case 5:
            std::cout << "Enter key: ";
            std::cin >> key;
            std::cout << "TTL: " << redis.ttl(key) << '\n';
            break;
        case 6: {
            const std::vector<std::string> keys = redis.keys();
            if (keys.empty()) {
                std::cout << "No keys stored.\n";
                break;
            }
            for (const std::string& storedKey : keys) {
                std::cout << storedKey << '\n';
            }
            break;
        }
        default:
            std::cout << "Invalid option.\n";
            break;
        }
    }

    return 0;
}