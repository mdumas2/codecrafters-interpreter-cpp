#pragma once

#include <vector>
#include <expected>
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
    std::string lexeme;
    std::string literal;
    int line;
};

extern std::unordered_map<std::string, TokenType> keywords;
extern std::unordered_map<std::string, TokenType> symbols;
extern std::unordered_map<TokenType, std::string> typeToString;

void init_token_maps();

class Scanner {
public:
    Scanner(std::string& src) : source(src), start(0), current(0), line(1) {}
    std::expected<std::vector<Token>, int> scan_tokens();

private:
    void add_token(TokenType type, const std::string& literal = "");
    void scan_string(int& ret_val);
    void scan_number();
    void scan_identifier();
    char advance();
    char peek();
    char peek_next();
    bool match(char expected);
    bool is_at_end();

    std::string source;
    size_t start;
    size_t current;
    int line;
    std::vector<Token> tokens;
};