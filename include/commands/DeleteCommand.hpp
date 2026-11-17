#pragma once

#include "ICommand.hpp"

class DeleteCommand : public ICommand {
    public:
        std::string execute(
            Redis& redis,
            const std::vector<std::string>& args
        ) override;
};
