#include "tokenizer.hpp"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>

std::pair<std::vector<Token>, int> Tokenizer::scan_tokens(std::string& src) {
    source = src;
    start = 0;
    current = 0;
    line = 1;
    int ret_val = 0;

    while (!is_at_end()) {
        start = current;
        char c = advance();

        switch (c) {
            case '(': add_token(TokenType::LEFT_PAREN); break;
            case ')': add_token(TokenType::RIGHT_PAREN); break;
            case '{': add_token(TokenType::LEFT_BRACE); break;
            case '}': add_token(TokenType::RIGHT_BRACE); break;
            case ',': add_token(TokenType::COMMA); break;
            case '.': add_token(TokenType::DOT); break;
            case '-': add_token(TokenType::MINUS); break;
            case '+': add_token(TokenType::PLUS); break;
            case ';': add_token(TokenType::SEMICOLON); break;
            case '*': add_token(TokenType::STAR); break;
            case '!': add_token(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
            case '=': add_token(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
            case '<': add_token(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
            case '>': add_token(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;
            case '/':
                if (match('/')) {
                    while (peek() != '\n' && !is_at_end()) advance();
                } else {
                    add_token(TokenType::SLASH);
                }
                break;
            case ' ':
            case '\r':
            case '\t':
                break;
            case '\n':
                line++;
                break;
            case '"': scan_string(ret_val); break;
            default:
                if (std::isdigit(c)) scan_number();
                else if (std::isalpha(c) || c == '_') scan_identifier();
                else {
                    std::cerr << "[line " << line << "] Error: Unexpected character: " << c << "\n";
                    ret_val = 65;
                }
                break;
        }
    }

    start = current;
    add_token(TokenType::EOF_TOKEN);
    return {tokens, ret_val};
}

std::string Tokenizer::get_name(TokenType type) {
    auto it = get_map().find(type);
    if (it != get_map().end()) {
        return it->second.name;
    }
    return "UNKNOWN";
}

TokenType Tokenizer::get_token_type_for_identifier(const std::string& word) {
    for (const auto& [type, info] : get_map()) {
        if (info.is_keyword && info.lexeme == word) {
            return type;
        }
    }
    return TokenType::IDENTIFIER;
}

void Tokenizer::add_token(TokenType type, const std::string& literal) {
    std::string lexeme = source.substr(start, current - start);
    tokens.push_back({type, get_name(type), lexeme, line});
}

char Tokenizer::advance() { return source[current++]; }
char Tokenizer::peek() { return is_at_end() ? '\0' : source[current]; }
char Tokenizer::peek_next() { return (current + 1 < source.size()) ? source[current + 1] : '\0'; }
bool Tokenizer::match(char expected) { if (is_at_end() || source[current] != expected) return false; current++; return true; }
bool Tokenizer::is_at_end() { return current >= source.size(); }

void Tokenizer::scan_string(int& ret_val) {
    while (peek() != '"' && !is_at_end()) {
        if (peek() == '\n') line++;
        advance();
    }

    if (is_at_end()) {
        std::cerr << "[line " << line << "] Error: Unterminated string.\n";
        ret_val = 65;
        return;
    }

    advance();
    std::string literal = source.substr(start + 1, current - start - 2);

    add_token(TokenType::STRING, literal);
}

void Tokenizer::scan_number() {
    while (std::isdigit(peek())) advance();

    bool is_float = false;
    if (peek() == '.' && std::isdigit(peek_next())) {
        is_float = true;
        advance();
        while (std::isdigit(peek())) advance();
    }

    std::string lexeme = source.substr(start, current - start);
    std::string literal;

    double value = std::stod(lexeme);

    std::ostringstream oss;
    oss << std::fixed;

    if (is_float && (!(std::floor(value) == value))) {
        oss << std::defaultfloat << lexeme;
        literal = oss.str();
    } else {
        oss << std::setprecision(1) << value;
        literal = oss.str();
    }

    add_token(TokenType::NUMBER, literal);
}

void Tokenizer::scan_identifier() {
    while (std::isalnum(peek()) || peek() == '_') advance();
    std::string text = source.substr(start, current - start);
    add_token(get_token_type_for_identifier(text));
}