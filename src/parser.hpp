#pragma once

#include <string>
#include <vector>
#include <variant>

#include "token.hpp"

struct LiteralExpr {
    Token value;
};

struct GroupingExpr {
    std::variant<LiteralExpr, GroupingExpr> expression;
};

using Expr = std::variant<LiteralExpr, GroupingExpr>;

class Parser {
public:
    Expr parse(const std::vector<Token>& tokens);

private:
    std::vector<Token> tokens;
    int current = 0;

    Expr expression();
    Expr literal();

    bool match(std::initializer_list<TokenType> types);
    bool check(TokenType type);
    const Token& advance();
    bool is_at_end();
    const Token& peek();
    const Token& previous();
};