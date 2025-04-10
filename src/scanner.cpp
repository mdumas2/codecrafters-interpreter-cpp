#include "scanner.hpp"

#include <iostream>
#include <unordered_set>
#include <algorithm>
#include <iomanip>

enum class TokenType {
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    IDENTIFIER, STRING, NUMBER,

    // Keywords
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

    ERROR, EOF_TOKEN
};

std::string tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::LEFT_PAREN:  return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
        case TokenType::LEFT_BRACE:  return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
        case TokenType::COMMA:       return "COMMA";
        case TokenType::DOT:         return "DOT";
        case TokenType::MINUS:       return "MINUS";
        case TokenType::PLUS:        return "PLUS";
        case TokenType::SEMICOLON:   return "SEMICOLON";
        case TokenType::SLASH:       return "SLASH";
        case TokenType::STAR:        return "STAR";
        case TokenType::BANG:        return "BANG";
        case TokenType::BANG_EQUAL:  return "BANG_EQUAL";
        case TokenType::EQUAL:       return "EQUAL";
        case TokenType::EQUAL_EQUAL: return "EQUAL_EQUAL";
        case TokenType::GREATER:     return "GREATER";
        case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
        case TokenType::LESS:        return "LESS";
        case TokenType::LESS_EQUAL:  return "LESS_EQUAL";
        case TokenType::IDENTIFIER:  return "IDENTIFIER";
        case TokenType::STRING:      return "STRING";
        case TokenType::NUMBER:      return "NUMBER";
        case TokenType::AND:         return "AND";
        case TokenType::CLASS:       return "CLASS";
        case TokenType::ELSE:        return "ELSE";
        case TokenType::FALSE:       return "FALSE";
        case TokenType::FOR:         return "FOR";
        case TokenType::FUN:         return "FUN";
        case TokenType::IF:          return "IF";
        case TokenType::NIL:         return "NIL";
        case TokenType::OR:          return "OR";
        case TokenType::PRINT:       return "PRINT";
        case TokenType::RETURN:      return "RETURN";
        case TokenType::SUPER:       return "SUPER";
        case TokenType::THIS:        return "THIS";
        case TokenType::TRUE:        return "TRUE";
        case TokenType::VAR:         return "VAR";
        case TokenType::WHILE:       return "WHILE";
        case TokenType::ERROR:       return "ERROR";
        case TokenType::EOF_TOKEN:         return "EOF";
        default:                return "UNKNOWN";
    }
}

struct Token {
    TokenType type;
    std::string lexeme;
    std::string literal;
    int line;
};

std::unordered_set<std::string> reservedWords = {
    "if", "else", "while", "for", "return", "true", "false", "class", "var", "fun", "print", "nil", "this", "super"
};

std::expected<std::vector<std::string>, int> tokenizer(std::string& file_content) {
    int ret_val = 0;
    int line_number = 1;
    std::vector<std::string> tokens;
    std::string value;
    size_t start;

    for (size_t i = 0; i < file_content.size(); ++i) {
        char c = file_content[i];

        switch (c) {
            case '(': tokens.push_back("LEFT_PAREN ( null"); break;
            case ')': tokens.push_back("RIGHT_PAREN ) null"); break;
            case '{': tokens.push_back("LEFT_BRACE { null"); break;
            case '}': tokens.push_back("RIGHT_BRACE } null"); break;
            case '.': tokens.push_back("DOT . null"); break;
            case ',': tokens.push_back("COMMA , null"); break;
            case ';': tokens.push_back("SEMICOLON ; null"); break;
            case '*': tokens.push_back("STAR * null"); break;
            case '-': tokens.push_back("MINUS - null"); break;
            case '+': tokens.push_back("PLUS + null"); break;
            case '=': 
                if (i + 1 < file_content.size() && file_content[i + 1] == '=') {
                    tokens.push_back("EQUAL_EQUAL == null");
                    i++;
                } else {
                    tokens.push_back("EQUAL = null");
                }
                break;
            case '!': 
                if (i + 1 < file_content.size() && file_content[i + 1] == '=') {
                    tokens.push_back("BANG_EQUAL != null");
                    i++;
                } else {
                    tokens.push_back("BANG ! null");
                }
                break;
            case '<': 
                if (i + 1 < file_content.size() && file_content[i + 1] == '=') {
                    tokens.push_back("LESS_EQUAL <= null");
                    i++;
                } else {
                    tokens.push_back("LESS < null");
                }
                break;
            case '>': 
                if (i + 1 < file_content.size() && file_content[i + 1] == '=') {
                    tokens.push_back("GREATER_EQUAL >= null");
                    i++;
                } else {
                    tokens.push_back("GREATER > null");
                }
                break;
            case '/': 
                if (i + 1 < file_content.size() && file_content[i + 1] == '/') {
                    i += 2;
                    while (i < file_content.size() && file_content[i] != '\n') {
                        i++;
                    }
                    ++line_number;
                } else {
                    tokens.push_back("SLASH / null");
                }
                break;
            case '"':
                start = i + 1;
                while (i + 1 < file_content.size() && file_content[i + 1] != '"') {
                    if (file_content[i + 1] == '\n') {
                        ++line_number;
                    }
                    i++;
                }
                i++;
                if (i == file_content.size()) {
                    std::cerr << "[line " << line_number << "] Error: Unterminated string." << std::endl;
                    ret_val = 65;
                    break;
                }
                value = file_content.substr(start, i - start);
                tokens.push_back("STRING \"" + value + "\" " + value);
                break;
            case '\t':
            case '\r':
            case ' ':
                break;
            case '\n':
                ++line_number;
                break;
            default:
                if (isdigit(c)) {
                    size_t start = i;
                    while (i + 1 < file_content.size() && (isdigit(file_content[i + 1]) || file_content[i + 1] == '.')) {
                        i++;
                    }
                    std::string number = file_content.substr(start, i - start + 1);
                    float num = std::stof(number);
                    std::ostringstream oss;
                    if (num == static_cast<int>(num)) {
                        oss << "NUMBER " << number << " " << std::fixed << std::setprecision(1) << num;
                    } else {
                        oss << "NUMBER " << number << " " << num;
                    }
                    tokens.push_back(oss.str());
                } else if (std::isalpha(c) || c == '_') {
                    size_t start = i;
                    while (i < file_content.size() && (std::isalnum(file_content[i]) || file_content[i] == '_')) {
                        ++i;
                    }
                    std::string identifier = file_content.substr(start, i - start);
                    if (reservedWords.find(identifier) != reservedWords.end()) {
                        std::string upper_identifier = identifier;
                        std::transform(upper_identifier.begin(), upper_identifier.end(), upper_identifier.begin(), ::toupper);
                        tokens.push_back(upper_identifier + " " + identifier + " null");
                    } else {
                        tokens.push_back("IDENTIFIER " + identifier + " null");
                    }
                    --i;
                } else {
                    std::cerr << "[line " << line_number << "] Error: Unexpected character: " << c << std::endl;
                    ret_val = 65;
                }
                break;
        }
    }

    tokens.push_back("EOF  null");

    if (ret_val == 0) {
        return tokens;
    } else {
        return std::unexpected(ret_val);
    }
}