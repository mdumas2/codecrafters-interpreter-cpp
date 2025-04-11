LiteralExpr Parser::parse(const std::vector<Token>& tks) {
    tokens = tks;
    current = 0;

    if (match(TokenType::LEFT_PAREN)) {
        LiteralExpr expr = expression();
        if (!match(TokenType::RIGHT_PAREN)) {
            throw std::runtime_error("Expected ')' after expression.");
        }

        GroupingExpr group{expr};
        return {Token{TokenType::STRING, "", "group " + expr.value.literal, 0}};
    }

    return literal();
}

LiteralExpr Parser::expression() {
    return literal();
}

LiteralExpr Parser::literal() {
    if (match(TokenType::STRING)) {
        return {previous()};
    }

    if (match(TokenType::TRUE) || match(TokenType::FALSE) || match(TokenType::NIL)) {
        return {previous()};
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