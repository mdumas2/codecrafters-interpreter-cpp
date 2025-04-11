#include <fstream>
#include <iostream>
#include <sstream>

#include "tokenizer.hpp"
#include "parser.hpp"

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

int main(int argc, char* argv[]) {
    std::cout << std::unitbuf;
     std::cerr << std::unitbuf;

    const std::string command = argv[1];
    std::string file_content = read_file_contents(argv[2]);

    Tokenizer tokenizer;
    auto [tokens, err] = tokenizer.scan_tokens(file_content);

    Parser parser;

    if (command == "tokenize") {
        for (const auto& token : tokens) {
            std::cout << lox::format_token(token) << std::endl;
        }
    } else if (command == "parse"){
        parser.parse(tokens);
    } else {
        std::cerr << "Unknown command: " << command << std::endl;
        return 1;
    }

    return err;
}