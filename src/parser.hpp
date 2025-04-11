#pragma once

#include <vector>
#include <expected>
#include <utility>

#include "token.hpp"

class Parser {
public:
    std::pair<std::vector<Token>, int> parser(std::string& src);

private:
};