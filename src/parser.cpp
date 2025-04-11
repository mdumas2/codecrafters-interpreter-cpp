#include "parser.hpp"
#include <stdexcept>

std::shared_ptr<Expr> Parser::parse(const std::vector<Token>& tks) {
    tokens = tks;
    return expression();
}

std::shared_ptr<Expr> Parser::expression() {
    if (match({TokenType::LEFT_PAREN})) {
        auto expr = expression();
        if (!match({TokenType::RIGHT_PAREN})) {
            throw std::runtime_error("Expected ')' after expression.");
        }
        return std::make_shared<GroupingExpr>(expr);
    }

    return literal();
}

std::shared_ptr<Expr> Parser::literal() {
    if (match({TokenType::STRING, TokenType::NUMBER,
               TokenType::TRUE, TokenType::FALSE, TokenType::NIL})) {
        return std::make_shared<LiteralExpr>(previous());
    }

    throw std::runtime_error("Expected literal value.");
}

bool Parser::match(std::initializer_list<TokenType> types) {
    for (auto type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::check(TokenType type) {
    if (is_at_end()) return false;
    return peek().type == type;
}

const Token& Parser::advance() {
    if (!is_at_end()) current++;
    return previous();
}

bool Parser::is_at_end() {
    return peek().type == TokenType::EOF_TOKEN;
}

const Token& Parser::peek() {
    return tokens[current];
}

const Token& Parser::previous() {
    return tokens[current - 1];
}