#pragma once

#include "token.hpp"

#include <vector>
#include <string>
#include <memory>
#include <any>

struct Binary;
struct Grouping;
struct Literal;
struct Unary;

template <typename R>
class ExprVisitor {
public:
    virtual ~ExprVisitor() = default;

    virtual R visitBinaryExpr(const Binary& expr) = 0;
    virtual R visitGroupingExpr(const Grouping& expr) = 0;
    virtual R visitLiteralExpr(const Literal& expr) = 0;
    virtual R visitUnaryExpr(const Unary& expr) = 0;
};

struct Expr {
    virtual ~Expr() = default;
    template <typename R>
    virtual R accept(ExprVisitor<R>& visitor) const = 0;
};

struct Binary : Expr {
    std::unique_ptr<Expr> left;
    Token op; 
    std::unique_ptr<Expr> right;

    Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

    template <typename R>
    R accept(ExprVisitor<R>& visitor) const override {
        return visitor.visitBinaryExpr(*this);
    }
};

struct Grouping : Expr {
    std::unique_ptr<Expr> expression;

    explicit Grouping(std::unique_ptr<Expr> expression)
        : expression(std::move(expression)) {}

    template <typename R>
    R accept(ExprVisitor<R>& visitor) const override {
        return visitor.visitGroupingExpr(*this);
    }
};

struct Literal : Expr {
    std::any value;

    explicit Literal(std::any value) : value(std::move(value)) {}

    template <typename R>
    R accept(ExprVisitor<R>& visitor) const override {
        return visitor.visitLiteralExpr(*this);
    }
};

struct Unary : Expr {
    Token op;
    std::unique_ptr<Expr> right;

    Unary(Token op, std::unique_ptr<Expr> right)
        : op(std::move(op)), right(std::move(right)) {}

    template <typename R>
    R accept(ExprVisitor<R>& visitor) const override {
        return visitor.visitUnaryExpr(*this);
    }
};