#pragma once

#include <string>
#include <vector>

#include "../core/core.hpp"

class ICommand {
    public:
        virtual ~ICommand() = default;

        virtual std::string execute(
            Redis& redis,
            const std::vector<std::string>& args
        ) = 0;
};
