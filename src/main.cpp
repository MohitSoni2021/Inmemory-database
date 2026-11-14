#include <iostream>
#include <limits>
#include <memory>
#include "../include/core/core.hpp"

namespace {

bool readKey(std::string& key) {
    std::cout << "Enter key: ";
    return static_cast<bool>(std::cin >> key);
}

bool readInteger(const std::string& prompt, long long& value) {
    std::cout << prompt;
    if (std::cin >> value) {
        return true;
    }

    std::cout << "Please enter a valid integer.\n";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return false;
}

RedisObject* findObject(Redis& redis, const std::string& key) {
    RedisObject* object = redis.getObject(key);
    if (object == nullptr) {
        std::cout << "Key not found.\n";
    }
    return object;
}

}

int main() {
    Redis redis;

    while (true) {
        std::cout << "\n1. Set string\n"
                  << "2. Set integer\n"
                  << "3. Create list\n"
                  << "4. Push to list\n"
                  << "5. Pop from list\n"
                  << "6. Create hash\n"
                  << "7. Set hash field\n"
                  << "8. Get hash field\n"
                  << "9. Get value\n"
                  << "10. Delete\n"
                  << "11. Set TTL\n"
                  << "12. Get TTL\n"
                  << "13. List keys\n"
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
            if (!readKey(key)) {
                return 1;
            }
            std::cout << "Enter value: ";
            std::cin >> value;
            redis.set(key, value);
            std::cout << "Value stored.\n";
            break;
        }
        case 2: {
            long long value;
            if (!readKey(key) || !readInteger("Enter integer: ", value)) {
                break;
            }
            if (value < std::numeric_limits<int>::min() ||
                value > std::numeric_limits<int>::max()) {
                std::cout << "Integer is outside the supported range.\n";
                break;
            }
            redis.set(key, static_cast<int>(value));
            std::cout << "Integer stored.\n";
            break;
        }
        case 3:
            if (!readKey(key)) {
                return 1;
            }
            redis.set(key, std::make_unique<RedisList>());
            std::cout << "List created.\n";
            break;
        case 4: {
            std::string value;
            if (!readKey(key)) {
                return 1;
            }
            std::cout << "Enter list value: ";
            std::cin >> value;

            RedisObject* object = findObject(redis, key);
            RedisList* list = dynamic_cast<RedisList*>(object);
            if (list == nullptr) {
                if (object != nullptr) {
                    std::cout << "Wrong type. Key does not contain a list.\n";
                }
                break;
            }
            list->lpush(value);
            std::cout << "Value pushed. List: " << list->toString() << '\n';
            break;
        }
        case 5: {
            if (!readKey(key)) {
                return 1;
            }

            RedisObject* object = findObject(redis, key);
            RedisList* list = dynamic_cast<RedisList*>(object);
            if (list == nullptr) {
                if (object != nullptr) {
                    std::cout << "Wrong type. Key does not contain a list.\n";
                }
                break;
            }

            const std::optional<std::string> value = list->lpop();
            std::cout << (value ? "Popped value: " + *value : "List is empty.") << '\n';
            break;
        }
        case 6:
            if (!readKey(key)) {
                return 1;
            }
            redis.set(key, std::make_unique<RedisHash>());
            std::cout << "Hash created.\n";
            break;
        case 7: {
            std::string field;
            std::string value;
            if (!readKey(key)) {
                return 1;
            }
            std::cout << "Enter field: ";
            std::cin >> field;
            std::cout << "Enter value: ";
            std::cin >> value;

            RedisObject* object = findObject(redis, key);
            RedisHash* hash = dynamic_cast<RedisHash*>(object);
            if (hash == nullptr) {
                if (object != nullptr) {
                    std::cout << "Wrong type. Key does not contain a hash.\n";
                }
                break;
            }
            hash->hset(field, value);
            std::cout << "Hash field stored.\n";
            break;
        }
        case 8: {
            std::string field;
            if (!readKey(key)) {
                return 1;
            }
            std::cout << "Enter field: ";
            std::cin >> field;

            RedisObject* object = findObject(redis, key);
            RedisHash* hash = dynamic_cast<RedisHash*>(object);
            if (hash == nullptr) {
                if (object != nullptr) {
                    std::cout << "Wrong type. Key does not contain a hash.\n";
                }
                break;
            }

            const std::optional<std::string> value = hash->hget(field);
            std::cout << (value ? "Field value: " + *value : "Field not found.") << '\n';
            break;
        }
        case 9: {
            if (!readKey(key)) {
                return 1;
            }
            const std::optional<std::string> value = redis.get(key);
            if (value) {
                RedisObject* object = redis.getObject(key);
                std::cout << "Type: " << object->type() << "\nValue: " << *value << '\n';
            } else {
                std::cout << "Key not found.\n";
            }
            break;
        }
        case 10:
            if (!readKey(key)) {
                return 1;
            }
            std::cout << (redis.del(key) ? "Key deleted.\n" : "Key not found.\n");
            break;
        case 11: {
            long long seconds;
            if (!readKey(key) || !readInteger("Enter TTL in seconds: ", seconds)) {
                break;
            }
            if (seconds < 0 || seconds > std::numeric_limits<int>::max()) {
                std::cout << "TTL must be between 0 and "
                          << std::numeric_limits<int>::max() << " seconds.\n";
                break;
            }
            std::cout << (redis.set_ttl(key, static_cast<int>(seconds))
                              ? "TTL set.\n"
                              : "Key not found.\n");
            break;
        }
        case 12:
            if (!readKey(key)) {
                return 1;
            }
            std::cout << "TTL: " << redis.ttl(key) << '\n';
            break;
        case 13: {
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