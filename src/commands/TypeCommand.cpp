#include "../../include/commands/TypeCommand.hpp"
#include <stdexcept>

std::string TypeCommand::execute(Redis &redis, const std::vector<std::string> &args) {
    if (args.size() != 1) {
        throw std::invalid_argument("TYPE command requires exactly one argument.");
    }

    return redis.typeOf(args[0]);
};

