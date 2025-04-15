#ifndef AST_PRINTER_HPP
#define AST_PRINTER_HPP

#include "expr.hpp"

namespace lox {

class AstPrinter : public ExprVisitor {
public:
    std::string print(const Expr& expr) {
        return expr.accept(*this);
    }

    std::string visitBinaryExpr(const Binary& expr) override {
        return parenthesize(expr.operator_.lexeme, {expr.left.get(), expr.right.get()});
    }

    std::string visitGroupingExpr(const Grouping& expr) override {
        return parenthesize("group", {expr.expression.get()});
    }

    std::string visitLiteralExpr(const Literal& expr) override {
        if (std::holds_alternative<std::monostate>(expr.value)) return "nil";
        if (std::holds_alternative<double>(expr.value))
            return std::to_string(std::get<double>(expr.value));
        if (std::holds_alternative<std::string>(expr.value))
            return std::get<std::string>(expr.value);
        if (std::holds_alternative<bool>(expr.value))
            return std::get<bool>(expr.value) ? "true" : "false";
        return "";
    }

    std::string visitUnaryExpr(const Unary& expr) override {
        return parenthesize(expr.operator_.lexeme, {expr.right.get()});
    }

private:
    std::string parenthesize(const std::string& name, const std::vector<const Expr*>& exprs) {
        std::string result = "(" + name;
        for (const auto* expr : exprs) {
            result += " ";
            result += expr->accept(*this);
        }
        result += ")";
        return result;
    }
};

} // namespace lox

#endif // AST_PRINTER_HPP