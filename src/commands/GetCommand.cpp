#include "../../include/commands/GetCommand.hpp"
#include <stdexcept>

std::string GetCommand::execute(Redis &redis, const std::vector<std::string> &args) {
    if(args.size() != 1) {
        throw std::invalid_argument(
            "GET command just required single argument."
        );
    };

    auto result = redis.get(args[0]);

    return result.value_or("(nil)");

}
