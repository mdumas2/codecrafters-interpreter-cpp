#pragma once

#include "expr.hpp"

#include <string>
#include <vector>
#include <sstream>

class AstPrinter : public ExprVisitor<std::string> {
private:
    template<typename... ExprArgs>
    std::string parenthesize(const std::string& name, const ExprArgs&... exprs);

    std::string parenthesizeList(const std::string& name, const std::vector<const Expr*>& exprs);

    std::string anyToString(const std::any& value) const;


public:
    std::string print(const Expr& expr);

    std::string visitBinaryExpr(const Binary& expr) override;
    std::string visitGroupingExpr(const Grouping& expr) override;
    std::string visitLiteralExpr(const Literal& expr) override;
    std::string visitUnaryExpr(const Unary& expr) override;
};


template<typename... ExprArgs>
std::string AstPrinter::parenthesize(const std::string& name, const ExprArgs&... exprs) {
    std::ostringstream builder;
    builder << "(" << name;
    ((builder << " " << exprs.accept(*this)), ...);
    builder << ")";
    return builder.str();
}