#include "scanner.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

std::string read_file_contents(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not open file: " << filename << std::endl;
        exit(1);
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

std::string format_token(const Token& token) {
    return token.name + " " + token.lexeme + " " + (token.literal.empty() ? "null" : token.literal);
}

std::string format_token_lexeme(const Token& token) {
    return token.literal.empty() ? token.lexeme : token.literal;
}

int main(int argc, char* argv[]) {
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    if (argc < 3) {
        std::cerr << "Usage: ./your_program tokenize <filename>" << std::endl;
        return 1;
    }

    const std::string command = argv[1];
    std::string file_content = read_file_contents(argv[2]);

    Scanner scanner;
    auto [tokens, err] = scanner.scan_tokens(file_content);

    if (command == "tokenize") {
        for (const auto& token : tokens) {
            std::cout << format_token(token) << std::endl;
        }
    } else if (command == "parse"){
        for (const auto& token : tokens) {
            std::cout << format_token_lexeme(token) << std::endl;
        }
    } else {
        std::cerr << "Unknown command: " << command << std::endl;
        return 1;
    }
    
    return err;
}