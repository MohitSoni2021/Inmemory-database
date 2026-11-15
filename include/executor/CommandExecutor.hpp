#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../commands/ICommand.hpp"

class CommandExecutor {
    private:
        std::unordered_map<
            std::string,
            std::unique_ptr<ICommand
        >> commands;

    public:
        CommandExecutor();

        std::string execute(
            Redis& redis,
            const std::vector<std::string>& tokens
        );
};
