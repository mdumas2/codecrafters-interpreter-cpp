#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <stdexcept>
#include "expr.hpp"
#include "token.hpp"

namespace lox {

class ParseError : public std::runtime_error {
public:
    ParseError(const std::string& message) : std::runtime_error(message) {}
};

class Parser {
public:
    Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}

    std::unique_ptr<Expr> parse() {
        try {
            return expression();
        } catch (const ParseError&) {
            return nullptr;
        }
    }

private:
    std::unique_ptr<Expr> expression() {
        return equality();
    }

    std::unique_ptr<Expr> equality() {
        auto expr = comparison();

        while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
            Token operator_ = previous();
            auto right = comparison();
            expr = std::make_unique<Binary>(std::move(expr), operator_, std::move(right));
        }

        return expr;
    }

    std::unique_ptr<Expr> comparison() {
        auto expr = term();

        while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})) {
            Token operator_ = previous();
            auto right = term();
            expr = std::make_unique<Binary>(std::move(expr), operator_, std::move(right));
        }

        return expr;
    }

    std::unique_ptr<Expr> term() {
        auto expr = factor();

        while (match({TokenType::MINUS, TokenType::PLUS})) {
            Token operator_ = previous();
            auto right = factor();
            expr = std::make_unique<Binary>(std::move(expr), operator_, std::move(right));
        }

        return expr;
    }

    std::unique_ptr<Expr> factor() {
        auto expr = unary();

        while (match({TokenType::SLASH, TokenType::STAR})) {
            Token operator_ = previous();
            auto right = unary();
            expr = std::make_unique<Binary>(std::move(expr), operator_, std::move(right));
        }

        return expr;
    }

    std::unique_ptr<Expr> unary() {
        if (match({TokenType::BANG, TokenType::MINUS})) {
            Token operator_ = previous();
            auto right = unary();
            return std::make_unique<Unary>(operator_, std::move(right));
        }

        return primary();
    }

    std::unique_ptr<Expr> primary() {
        if (match({TokenType::FALSE})) return std::make_unique<Literal>(Literal::Value{false});
        if (match({TokenType::TRUE})) return std::make_unique<Literal>(Literal::Value{true});
        if (match({TokenType::NIL})) return std::make_unique<Literal>(Literal::Value{std::monostate{}});

        if (match({TokenType::NUMBER})) {
            try {
                double value = std::stod(previous().literal);
                return std::make_unique<Literal>(Literal::Value{value});
            } catch (...) {
                throw error(previous(), "Invalid number literal.");
            }
        }

        if (match({TokenType::STRING})) {
            return std::make_unique<Literal>(Literal::Value{previous().literal});
        }

        if (match({TokenType::LEFT_PAREN})) {
            auto expr = expression();
            consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
            return std::make_unique<Grouping>(std::move(expr));
        }

        throw error(peek(), "Expect expression.");
    }

    bool match(const std::vector<TokenType>& types) {
        for (const auto& type : types) {
            if (check(type)) {
                advance();
                return true;
            }
        }
        return false;
    }

    Token consume(TokenType type, const std::string& message) {
        if (check(type)) return advance();
        throw error(peek(), message);
    }

    bool check(TokenType type) const {
        if (isAtEnd()) return false;
        return peek().type == type;
    }

    Token advance() {
        if (!isAtEnd()) ++current;
        return previous();
    }

    bool isAtEnd() const {
        return peek().type == TokenType::EOF_TOKEN;
    }

    Token peek() const {
        return tokens[current];
    }

    Token previous() const {
        return tokens[current - 1];
    }

    ParseError error(const Token& token, const std::string& message) const {
        // Assuming Lox::error is defined elsewhere to report errors
        Lox::error(token, message);
        return ParseError(message);
    }

    void synchronize() {
        advance();

        while (!isAtEnd()) {
            if (previous().type == TokenType::SEMICOLON) return;

            switch (peek().type) {
                case TokenType::CLASS:
                case TokenType::FUN:
                case TokenType::VAR:
                case TokenType::FOR:
                case TokenType::IF:
                case TokenType::WHILE:
                case TokenType::PRINT:
                case TokenType::RETURN:
                    return;
                default:
                    break;
            }

            advance();
        }
    }

    const std::vector<Token>& tokens;
    size_t current;
};

} // namespace lox

#endif // PARSER_HPP