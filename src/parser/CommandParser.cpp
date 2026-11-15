#include "../../include/parser/CommandParser.hpp"
#include <sstream>

std::vector<std::string> CommandParser::parse(const std::string &input) {
    std::vector<std::string> tokens;
    std::istringstream stream(input);

    std::string token;

    while(stream >> token) {
        tokens.push_back(token);
    };

    return tokens;
}
