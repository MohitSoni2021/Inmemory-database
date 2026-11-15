#include "../../include/commands/SetCommand.hpp"

#include <charconv>
#include <system_error>
#include <stdexcept>

std::string SetCommand::execute(Redis &redis, const std::vector<std::string> &args) {
    if(args.size() < 2) {
        throw std::invalid_argument(
            "SET requires atleast two arguments."
        );
    };

    const std::string& key = args[0];
    const std::string& value = args[1];

//    let's handle for the interger inputs;
    int number{};

    auto [ptr, error] = std::from_chars(
        value.data(),
        value.data() + value.size(),
        number
    );

    if(error == std::errc{} && ptr == value.data() + value.size() ) {
        redis.set(key, number);
    }else {
        redis.set(key, value);
    }

    return "+OK";
}
