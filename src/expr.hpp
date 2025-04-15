#ifndef EXPR_HPP
#define EXPR_HPP

#include <memory>
#include <string>
#include <variant>
#include <vector>
#include "token.hpp"

namespace lox {

class Expr;
class Binary;
class Grouping;
class Literal;
class Unary;

// Visitor interface for the Visitor pattern
class ExprVisitor {
public:
    virtual ~ExprVisitor() = default;
    virtual std::string visitBinaryExpr(const Binary& expr) = 0;
    virtual std::string visitGroupingExpr(const Grouping& expr) = 0;
    virtual std::string visitLiteralExpr(const Literal& expr) = 0;
    virtual std::string visitUnaryExpr(const Unary& expr) = 0;
};

// Base expression class
class Expr {
public:
    virtual ~Expr() = default;
    virtual std::string accept(ExprVisitor& visitor) const = 0;
};

// Binary expression: left operator right
class Binary : public Expr {
public:
    Binary(std::unique_ptr<Expr> left, Token operator_, std::unique_ptr<Expr> right)
        : left(std::move(left)), operator_(operator_), right(std::move(right)) {}

    std::string accept(ExprVisitor& visitor) const override {
        return visitor.visitBinaryExpr(*this);
    }

    const std::unique_ptr<Expr> left;
    const Token operator_;
    const std::unique_ptr<Expr> right;
};

// Grouping expression: (expression)
class Grouping : public Expr {
public:
    Grouping(std::unique_ptr<Expr> expression)
        : expression(std::move(expression)) {}

    std::string accept(ExprVisitor& visitor) const override {
        return visitor.visitGroupingExpr(*this);
    }

    const std::unique_ptr<Expr> expression;
};

// Literal expression: number, string, true, false, nil
class Literal : public Expr {
public:
    using Value = std::variant<std::monostate, double, std::string, bool>;

    Literal(Value value) : value(value) {}

    std::string accept(ExprVisitor& visitor) const override {
        return visitor.visitLiteralExpr(*this);
    }

    const Value value;
};

// Unary expression: operator expression
class Unary : public Expr {
public:
    Unary(Token operator_, std::unique_ptr<Expr> right)
        : operator_(operator_), right(std::move(right)) {}

    std::string accept(ExprVisitor& visitor) const override {
        return visitor.visitUnaryExpr(*this);
    }

    const Token operator_;
    const std::unique_ptr<Expr> right;
};

} // namespace lox

#endif // EXPR_HPP