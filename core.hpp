#include <unordered_map>
#include <string>
#include <vector>
#include <optional>

class Redis {
    private:
        std::unordered_map<std::string, std::string> data;
    
    public:
        void set(const std::string& key, const std::string& value) {
            data[key] = value;
        };

        std::optional<std::string> get(const std::string& key) {
            auto it = data.find(key);
            if(it != data.end()) {
                return it->second;
            } else{
                return std::nullopt;
            }
        }

        std::vector<std::string> keys() {
            std::vector<std::string> keys;
            for (const auto& pair : data) {
                keys.push_back(pair.first);
            };
            return keys;
        };

};