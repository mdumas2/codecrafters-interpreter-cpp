#pragma once

#include <string>
#include <vector>

#include "token.hpp"


struct LiteralExpr {
    Token value;
};

struct GroupingExpr {
    LiteralExpr expression;
};


class Parser {
public:
    LiteralExpr parse(const std::vector<Token>& tokens);

private:
    std::vector<Token> tokens;
    int current = 0;

    LiteralExpr expression();
    LiteralExpr literal();

    bool match(TokenType type);
    bool check(TokenType type);
    const Token& advance();
    bool is_at_end();
    const Token& peek();
    const Token& previous();
};