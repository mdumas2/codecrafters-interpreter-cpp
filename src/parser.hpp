#pragma once

#include <string>
#include <vector>

#include "token.hpp"


struct LiteralExpr;
struct GroupingExpr;

enum class ExprType {
    Literal,
    Grouping
};

struct Expr {
    ExprType type;

    union {
        LiteralExpr* literal;
        GroupingExpr* grouping;
    };

    std::string to_string() const;
};

struct LiteralExpr {
    Token value;

    std::string to_string() const {
        return value.literal;
    }
};

struct GroupingExpr {
    Expr expression;

    std::string to_string() const {
        return "(group " + expression.to_string() + ")";
    }
};

inline std::string Expr::to_string() const {
    switch (type) {
        case ExprType::Literal:
            return literal->to_string();
        case ExprType::Grouping:
            return grouping->to_string();
        default:
            return "";
    }
}


class Parser {
public:
    Expr parse(const std::vector<Token>& tokens);

private:
    std::vector<Token> tokens;
    int current = 0;

    Expr expression();
    Expr literal();

    bool match(TokenType type);
    bool check(TokenType type);
    const Token& advance();
    bool is_at_end();
    const Token& peek();
    const Token& previous();
};