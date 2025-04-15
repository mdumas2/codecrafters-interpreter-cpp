#pragma once

#include <vector>
#include <expected>
#include <utility>

#include "token.hpp"

class Tokenizer {
public:
    std::pair<std::vector<Token>, int> scan_tokens(std::string& src);
    std::string get_name(TokenType type);

private:
    void add_token(TokenType type, const std::string& literal = "");
    TokenType get_token_type_for_identifier(const std::string& word);
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