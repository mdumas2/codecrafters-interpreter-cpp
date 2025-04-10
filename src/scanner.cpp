#include <iostream>
#include <string>
#include <unordered_set>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <cctype>

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