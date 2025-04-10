#include "scanner.hpp"

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>

struct TokenInfo {
    std::string name;
    std::string lexeme;
    bool is_keyword;
};

std::unordered_map<TokenType, TokenInfo> TOKEN_INFO_MAP = {
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

std::expected<std::vector<Token>, int> Scanner::scan_tokens(std::string& src) {
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
    return ret_val == 0 ? std::expected<std::vector<Token>, int>{tokens} : std::unexpected(ret_val);
}

std::string Scanner::get_name(TokenType type) {
    auto it = TOKEN_INFO_MAP.find(type);
    if (it != TOKEN_INFO_MAP.end()) {
        return it->second.name;
    }
    return "UNKNOWN";
}

TokenType Scanner::get_token_type_for_identifier(const std::string& word) {
    for (const auto& [type, info] : TOKEN_INFO_MAP) {
        if (info.is_keyword && info.lexeme == word) {
            return type;
        }
    }
    return TokenType::IDENTIFIER;
}

void Scanner::add_token(TokenType type, const std::string& literal) {
    std::string lexeme = source.substr(start, current - start);
    tokens.push_back({type, get_name(type), lexeme, literal, line});
}

char Scanner::advance() { return source[current++]; }
char Scanner::peek() { return is_at_end() ? '\0' : source[current]; }
char Scanner::peek_next() { return (current + 1 < source.size()) ? source[current + 1] : '\0'; }
bool Scanner::match(char expected) { if (is_at_end() || source[current] != expected) return false; current++; return true; }
bool Scanner::is_at_end() { return current >= source.size(); }

void Scanner::scan_string(int& ret_val) {
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
    std::string value = source.substr(start + 1, current - start - 2);
    add_token(TokenType::STRING, value);
}

void Scanner::scan_number() {
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

void Scanner::scan_identifier() {
    while (std::isalnum(peek()) || peek() == '_') advance();
    std::string text = source.substr(start, current - start);
    add_token(get_token_type_for_identifier(text));
}