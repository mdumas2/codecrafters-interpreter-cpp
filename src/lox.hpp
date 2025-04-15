#ifndef LOX_HPP
#define LOX_HPP

#include "token.hpp"
#include <iostream>
#include <string>

namespace lox {

class Lox {
public:
    static void error(const Token& token, const std::string& message) {
        if (token.type == TokenType::EOF_TOKEN) {
            report(token.line, " at end", message);
        } else {
            report(token.line, " at '" + token.lexeme + "'", message);
        }
    }

private:
    static void report(int line, const std::string& where, const std::string& message) {
        std::cerr << "[line " << line << "] Error" << where << ": " << message << std::endl;
        hadError = true;
    }

public:
    static bool hadError;
};

bool Lox::hadError = false;

} // namespace lox

#endif // LOX_HPP