#include "scanner.hpp"

#include <iostream>
#include <iomanip>
#include <algorithm>

struct TokenInfo {
    TokenType type;
    std::string name;
    std::string lexeme;
    bool is_keyword;
};

const std::vector<TokenInfo> TOKEN_TABLE = {
    {TokenType::LEFT_PAREN,  "LEFT_PAREN",  "(",   false},
    {TokenType::RIGHT_PAREN, "RIGHT_PAREN", ")",   false},
    {TokenType::LEFT_BRACE,  "LEFT_BRACE",  "{",   false},
    {TokenType::RIGHT_BRACE, "RIGHT_BRACE", "}",   false},
    {TokenType::COMMA,       "COMMA",       ",",   false},
    {TokenType::DOT,         "DOT",         ".",   false},
    {TokenType::MINUS,       "MINUS",       "-",   false},
    {TokenType::PLUS,        "PLUS",        "+",   false},
    {TokenType::SEMICOLON,   "SEMICOLON",   ";",   false},
    {TokenType::SLASH,       "SLASH",       "/",   false},
    {TokenType::STAR,        "STAR",        "*",   false},
    {TokenType::BANG,        "BANG",        "!",   false},
    {TokenType::BANG_EQUAL,  "BANG_EQUAL",  "!=",  false},
    {TokenType::EQUAL,       "EQUAL",       "=",   false},
    {TokenType::EQUAL_EQUAL, "EQUAL_EQUAL", "==",  false},
    {TokenType::GREATER,     "GREATER",     ">",   false},
    {TokenType::GREATER_EQUAL, "GREATER_EQUAL", ">=", false},
    {TokenType::LESS,        "LESS",        "<",   false},
    {TokenType::LESS_EQUAL,  "LESS_EQUAL",  "<=",  false},

    {TokenType::AND,   "AND",   "and",   true},
    {TokenType::CLASS, "CLASS", "class", true},
    {TokenType::ELSE,  "ELSE",  "else",  true},
    {TokenType::FALSE, "FALSE", "false", true},
    {TokenType::FUN,   "FUN",   "fun",   true},
    {TokenType::FOR,   "FOR",   "for",   true},
    {TokenType::IF,    "IF",    "if",    true},
    {TokenType::NIL,   "NIL",   "nil",   true},
    {TokenType::OR,    "OR",    "or",    true},
    {TokenType::PRINT, "PRINT", "print", true},
    {TokenType::RETURN,"RETURN","return",true},
    {TokenType::SUPER, "SUPER", "super", true},
    {TokenType::THIS,  "THIS",  "this",  true},
    {TokenType::TRUE,  "TRUE",  "true",  true},
    {TokenType::VAR,   "VAR",   "var",   true},
    {TokenType::WHILE, "WHILE", "while", true},

    {TokenType::EOF_TOKEN, "EOF", "", false},
};

std::unordered_map<std::string, TokenType> keywords;
std::unordered_map<std::string, TokenType> symbols;
std::unordered_map<TokenType, std::string> typeToString;

void init_token_maps() {
    for (const auto& token : TOKEN_TABLE) {
        typeToString[token.type] = token.name;
        if (token.is_keyword) {
            keywords[token.lexeme] = token.type;
        } else if (!token.lexeme.empty()) {
            symbols[token.lexeme] = token.type;
        }
    }
}

std::expected<std::vector<Token>, int> Scanner::scan_tokens() {
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

void Scanner::add_token(TokenType type, const std::string& literal) {
    std::string lexeme = source.substr(start, current - start);
    tokens.push_back({type, lexeme, literal, line});
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
    if (peek() == '.' && std::isdigit(peek_next())) {
        advance();
        while (std::isdigit(peek())) advance();
    }
    std::string number = source.substr(start, current - start);
    add_token(TokenType::NUMBER, number);
}

void Scanner::scan_identifier() {
    while (std::isalnum(peek()) || peek() == '_') advance();
    std::string text = source.substr(start, current - start);
    std::cout << "Text is: " << text << std::endl;
    add_token(TokenType::IDENTIFIER, keywords[text]);
}