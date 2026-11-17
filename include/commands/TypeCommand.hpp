#pragma once

#include "ICommand.hpp"

class TypeCommand : public ICommand {
    public:
        std::string execute(
            Redis& redis,
            const std::vector<std::string>& args
        ) override;
};

