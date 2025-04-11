#pragma once

#include <memory>
#include <string>

#include "token.hpp"

struct Expr {
    virtual ~Expr() = default;
};

struct LiteralExpr : Expr {
    Token value;
    explicit LiteralExpr(const Token& value) : value(value) {}
};

struct GroupingExpr : Expr {
    std::shared_ptr<Expr> expression;
    explicit GroupingExpr(std::shared_ptr<Expr> expr) : expression(expr) {}
};

class Parser {
public:
    std::shared_ptr<Expr> parse(const std::vector<Token>& tks);

private:
    std::vector<Token>& tokens;
    int current = 0;

    std::shared_ptr<Expr> expression();
    std::shared_ptr<Expr> literal();

    const Token& peek();
    const Token& previous();
    const Token& advance();
    bool is_at_end();
    bool check(TokenType type);
    bool match(std::initializer_list<TokenType> types);
};