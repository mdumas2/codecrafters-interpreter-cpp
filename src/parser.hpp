#pragma once

#include "token.hpp"
#include "expr.hpp"

#include <vector>
#include <stdexcept>

class Parser {
private:
    class ParseError : public std::runtime_error {
    public:
        ParseError() : std::runtime_error("") {}
    };

    const std::vector<Token>& tokens;
    int current = 0;

    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> primary();

    bool match(const std::vector<TokenType>& types);

    Token consume(TokenType type, const std::string& message);

    bool check(TokenType type) const;

    Token advance();

    bool isAtEnd() const;

    Token peek() const;

    Token previous() const;

    ParseError error(const Token& token, const std::string& message);

    void synchronize();


public:
    explicit Parser(const std::vector<Token>& tokens);

    std::unique_ptr<Expr> parse();
};