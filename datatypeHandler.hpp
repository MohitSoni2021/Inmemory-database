#include <string>
#include <deque>
#include <unordered_map>
#include <optional>
#include <memory>

// Abstract base class for handling different data types;
class RedisObject {
    public:
        virtual ~RedisObject() = default;
        virtual std::string type() const = 0;
        virtual std::string toString() const = 0;
};

// String data type handler;
class RedisString : public RedisObject {
    private:
        std::string value;
    
    public:
        explicit RedisString(const std::string& val) : value(val) {};
        std::string type() const override { return "string"; };
        std::string toString() const override { return value; };
} ;

//  Building for the integer data type handler;
class RedisInteger : public RedisObject {
    private:
        long long value;

    public:
        explicit RedisInteger(long long val) : value(val) {};
        std::string type() const override { return "integer"; };
        std::string toString() const override { return std::to_string(value); };

        long long getValue() const { return value; };
};

//  List data type handler;
class RedisList : public RedisObject {
    private:
        std::deque<std::string> value;

    public:
        RedisList() = default;

        std::string type() const override { return "list"; };
        std::string toString() const override {
            std::string result = "[";
            for (size_t i = 0; i < value.size(); ++i) {
                result += value[i];
                if(i < value.size() - 1) {
                    result += ", ";
                }
            };
            return result + "]";
        };

        // List operations;
        void lpush(const std::string& val) {
            value.push_front(val);
        };

        std::optional<std::string> lpop() {
            if(value.empty()) return std::nullopt;
            // getting the cval
            std::string fVal = value.front();
            value.pop_front();
            return fVal;
        };

        int size() const {
            return value.size();
        }
};

// Hash data type handler;
// this acts as JSON or dictionary;
class RedisHash : public RedisObject {
    private:
        std::unordered_map<std::string, std::string> obj;
    
    public:
        RedisHash() = default;
        std::string type() const override { return "Hash"; };
        std::string toString() const override {
            std::string res = "{";
            bool first = true;
            for (const auto& pair : obj) {
                if (!first) res += ", ";
                res += "\"" + pair.first + "\": \"" + pair.second + "\"";
                first = false;
            }
            return res + "}";
        };

        // Object specific operations;
        void hset(const std::string& field, const std::string& value) {
            obj[field] = value;
        };

        std::optional<std::string> hget(const std::string& field) {
            auto it = obj.find(field);
            if(it != obj.end()) { return it->second; }
            return std::nullopt;
        };

        bool isEmpty() const { return obj.empty(); };
};