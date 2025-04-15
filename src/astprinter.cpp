#include "astprinter.hpp"
#include <typeinfo>
#include <stdexcept>

std::string AstPrinter::print(const Expr& expr) {
    return expr.accept(*this);
}

std::string AstPrinter::visitBinaryExpr(const Binary& expr) {
    return parenthesize(expr.op.lexeme, *expr.left, *expr.right);
}

std::string AstPrinter::visitGroupingExpr(const Grouping& expr) {
    return parenthesize("group", *expr.expression);
}

std::string AstPrinter::visitLiteralExpr(const Literal& expr) {
     return anyToString(expr.value);
}

std::string AstPrinter::visitUnaryExpr(const Unary& expr) {
    return parenthesize(expr.op.lexeme, *expr.right);
}

std::string AstPrinter::parenthesizeList(const std::string& name, const std::vector<const Expr*>& exprs) {
    std::ostringstream builder;
    builder << "(" << name;
    for (const Expr* expr_ptr : exprs) {
        if (expr_ptr) {
             builder << " " << expr_ptr->accept(*this);
        } else {
             builder << " <null_expr>";
        }
    }
    builder << ")";
    return builder.str();
}


std::string AstPrinter::anyToString(const std::any& value) const {
    if (!value.has_value()) return "nil";

    try {
        if (value.type() == typeid(std::nullptr_t)) {
            return "nil";
        } else if (value.type() == typeid(std::string)) {
            return std::any_cast<std::string>(value);
        } else if (value.type() == typeid(double)) {
            std::string numStr = std::to_string(std::any_cast<double>(value));
             size_t decimalPoint = numStr.find('.');
             if (decimalPoint != std::string::npos) {
                numStr.erase(numStr.find_last_not_of('0') + 1, std::string::npos);
                if (numStr.back() == '.') {
                    numStr.pop_back();
                }
             }
            return numStr;
        } else if (value.type() == typeid(bool)) {
            return std::any_cast<bool>(value) ? "true" : "false";
        } else {
            return "unknown literal type";
        }
    } catch (const std::bad_any_cast& e) {
        return "bad_any_cast: " + std::string(e.what());
    }
}