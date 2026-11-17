#include "../../include/commands/DeleteCommand.hpp"
#include <stdexcept>

std::string DeleteCommand::execute(Redis &redis, const std::vector<std::string> &args) {
    if(args.size() != 1) std::invalid_argument("Delete function require only single argument.");

    auto result = redis.del(args[0]);

    return result ? "1" : "key not found.";
};
