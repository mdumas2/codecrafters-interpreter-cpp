#include "token.hpp"

using namespace lox;

const std::unordered_map<TokenType, TokenInfo>& get_map() {
    static const std::unordered_map<TokenType, TokenInfo> map = {
        {TokenType::LEFT_PAREN,  {"LEFT_PAREN",  "(",   false}},
        {TokenType::RIGHT_PAREN, {"RIGHT_PAREN", ")",   false}},
        {TokenType::LEFT_BRACE,  {"LEFT_BRACE",  "{",   false}},
        {TokenType::RIGHT_BRACE, {"RIGHT_BRACE", "}",   false}},
        {TokenType::COMMA,       {"COMMA",       ",",   false}},
        {TokenType::DOT,         {"DOT",         ".",   false}},
        {TokenType::MINUS,       {"MINUS",       "-",   false}},
        {TokenType::PLUS,        {"PLUS",        "+",   false}},
        {TokenType::SEMICOLON,   {"SEMICOLON",   ";",   false}},
        {TokenType::SLASH,       {"SLASH",       "/",   false}},
        {TokenType::STAR,        {"STAR",        "*",   false}},
        {TokenType::BANG,        {"BANG",        "!",   false}},
        {TokenType::BANG_EQUAL,  {"BANG_EQUAL",  "!=",  false}},
        {TokenType::EQUAL,       {"EQUAL",       "=",   false}},
        {TokenType::EQUAL_EQUAL, {"EQUAL_EQUAL", "==",  false}},
        {TokenType::GREATER,     {"GREATER",     ">",   false}},
        {TokenType::GREATER_EQUAL, {"GREATER_EQUAL", ">=", false}},
        {TokenType::LESS,        {"LESS",        "<",   false}},
        {TokenType::LESS_EQUAL,  {"LESS_EQUAL",  "<=",  false}},

        {TokenType::AND,   {"AND",   "and",   true}},
        {TokenType::CLASS, {"CLASS", "class", true}},
        {TokenType::ELSE,  {"ELSE",  "else",  true}},
        {TokenType::FALSE, {"FALSE", "false", true}},
        {TokenType::FUN,   {"FUN",   "fun",   true}},
        {TokenType::FOR,   {"FOR",   "for",   true}},
        {TokenType::IF,    {"IF",    "if",    true}},
        {TokenType::NIL,   {"NIL",   "nil",   true}},
        {TokenType::OR,    {"OR",    "or",    true}},
        {TokenType::PRINT, {"PRINT", "print", true}},
        {TokenType::RETURN,{"RETURN","return",true}},
        {TokenType::SUPER, {"SUPER", "super", true}},
        {TokenType::THIS,  {"THIS",  "this",  true}},
        {TokenType::TRUE,  {"TRUE",  "true",  true}},
        {TokenType::VAR,   {"VAR",   "var",   true}},
        {TokenType::WHILE, {"WHILE", "while", true}},

        {TokenType::IDENTIFIER, {"IDENTIFIER", "", false}},
        {TokenType::STRING,     {"STRING",     "", false}},
        {TokenType::NUMBER,     {"NUMBER",     "", false}},

        {TokenType::EOF_TOKEN, {"EOF", "", false}},
    };
    return map;
}
