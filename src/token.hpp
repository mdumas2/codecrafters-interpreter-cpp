#pragma once

#include <string>
#include <unordered_map>

enum class TokenType {
    // Symbols
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

    // Operators
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // Literals
    IDENTIFIER, STRING, NUMBER,

    // Keywords
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

    ERROR, EOF_TOKEN
};

struct Token {
    TokenType type;
    std::string name;
    std::string lexeme;
    std::string literal;
    int line;
};

struct TokenInfo {
    const std::string name;
    const std::string lexeme;
    bool is_keyword;
};

namespace lox {
    const std::unordered_map<TokenType, TokenInfo>& get_map();
}