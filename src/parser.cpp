#include "parser.hpp"

#include <stdexcept>

Expr Parser::parse(const std::vector<Token>& tks) {
    tokens = tks;
    current = 0;

    return expression();
}

Expr Parser::expression() {
    if (match(TokenType::LEFT_PAREN)) {
        Expr expr = expression();
        if (!match(TokenType::RIGHT_PAREN)) {
            throw std::runtime_error("Expected ')' after expression.");
        }

        auto* group = new GroupingExpr{expr};
        return Expr{ExprType::Grouping, .grouping = group};
    }

    return literal();
}

Expr Parser::literal() {
    if (match(TokenType::STRING) || match(TokenType::TRUE) || match(TokenType::FALSE) || match(TokenType::NIL)) {
        auto* lit = new LiteralExpr{previous()};
        return Expr{ExprType::Literal, .literal = lit};
    }

    throw std::runtime_error("Expected literal value.");
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
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