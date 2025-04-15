#pragma once

#include "token.hpp"

#include <string>
#include <iostream>

namespace lox {
    extern bool hadError;

    inline void report(int line, const std::string& where, const std::string& message) {
        std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
        hadError = true;
    }

    inline void error(int line, const std::string& message) {
        report(line, "", message);
    }

    inline void error(const Token& token, const std::string& message) {
        if (token.type == TokenType::EOF_TOKEN) {
            report(token.line, " at end", message);
        } else {
            report(token.line, " at '" + token.lexeme + "'", message);
        }
    }

} /