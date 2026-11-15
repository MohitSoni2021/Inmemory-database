#include "../../include/executor/CommandExecutor.hpp"

#include "../../include/commands/SetCommand.hpp"
#include "../../include/commands/GetCommand.hpp"

#include <algorithm>
#include <cctype>
#include <stdexcept>

// writing the implimentation for the constructor for the CommandExecutor.hpp file;
CommandExecutor::CommandExecutor() {
    commands["SET"] = std::make_unique<SetCommand>();
    commands["GET"] = std::make_unique<GetCommand>();
};

// Now building the execute function;
std::string CommandExecutor::execute(Redis& redis, const std::vector<std::string> &tokens) {
    if(tokens.empty()) {
        throw std::invalid_argument("Empty Command..");
    };

    // getting the command from the user terminal input;
    std::string command = tokens[0];
    // Transforming the starting command to the uppercase;
    std::transform(
        command.begin(),
        command.end(),
        command.begin(),
        [](unsigned char c) {
            return static_cast<char>(std::toupper(c));
        }
    );

    // Finding the command from the command map;
    auto it = commands.find(command);

    // handling if command not found;
    if(it == commands.end()) throw std::invalid_argument("Invalid Command.");

    // Building the commands;
    std::vector<std::string> args(tokens.begin()+1, tokens.end());

    // Executing the command using polymorphism;
    return it->second->execute(redis, args);

}

