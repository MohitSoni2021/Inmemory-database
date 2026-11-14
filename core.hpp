#include <unordered_map>
#include <string>
#include <vector>
#include <optional>
#include <chrono>
#include <memory>
#include <stdexcept>
#include <utility>
#include "datatypeHandler.hpp"

// building the alice;
using timePoint = std::chrono::time_point<std::chrono::system_clock>;

struct CacheEntry {
    std::unique_ptr<RedisObject> value;
    std::optional<timePoint> ttl; // this is optional because some keys may not have a ttl;
};

class Redis {
    private:
        std::unordered_map<std::string, CacheEntry> data;
        timePoint now() const {
            // getting the current system time;
            return std::chrono::system_clock::now();
        }
    
    public:
        void set(const std::string& key, std::unique_ptr<RedisObject> value) {
            if (!value) {
                throw std::invalid_argument("Redis value cannot be null");
            }
            data[key] = CacheEntry{std::move(value), std::nullopt};
        };

        void set(const std::string& key, const std::string& value) {
            set(key, std::make_unique<RedisString>(value));
        };

        void set(const std::string& key, const int value) {
            set(key, std::make_unique<RedisInteger>(value));
        };

        std::optional<std::string> get(const std::string& key) {
            auto it = data.find(key);
            if(it != data.end()) {
                // checking if the key has a ttl and if it has expired;
                if(it->second.ttl.has_value() && it->second.ttl.value() <= now() ) {
                    data.erase(it); // remove if the key has expired;
                    return std::nullopt;
                };
                return it->second.value->toString();
            } else{
                return std::nullopt;
            }
        };

        RedisObject* getObject(const std::string& key) {
            auto it = data.find(key);
            if (it == data.end()) {
                return nullptr;
            }

            if (it->second.ttl.has_value() && it->second.ttl.value() <= now()) {
                data.erase(it);
                return nullptr;
            }

            return it->second.value.get();
        }

        // delete command;
        bool del(const std::string& key) {
            //  return true if the key was found and deleted, false otherwise;
            return data.erase(key) > 0;
        };

        // Set ttl command;
        bool set_ttl(const std::string& key, int seconds) {
            auto it = data.find(key);
            if(it != data.end()) {
                it->second.ttl = now() + std::chrono::seconds(seconds);
                return true;
            } else {
                return false;
            }
        }

        std::vector<std::string> keys() {
            std::vector<std::string> keys;
            for (auto it = data.begin(); it != data.end();) {
                if(it->second.ttl.has_value() && it->second.ttl.value() <= now()) {
                    it = data.erase(it);
                } else {
                    keys.push_back(it->first);
                    ++it;
                }
            }
            return keys;
        };

        int ttl(const std::string& key) {
            // returning value rules;
            // -1 if key does not have the ttl value means forever key;
            //  -2 if key does not exist;
            auto it = data.find(key);

            if(it != data.end()) {
                if(it->second.ttl.has_value()) {
                    // time left in seconds;
                    auto time_left = std::chrono::duration_cast<std::chrono::seconds>(it->second.ttl.value() - now()).count();

                    if(time_left <= 0) {
                        data.erase(it);
                        return -2; // key has expired and removed;
                    }

                    return int(time_left);

                }else {
                    return -1; // key does not have a ttl value;
                }
            } else {
                return -2; // key does not exist;
            }
        }

};