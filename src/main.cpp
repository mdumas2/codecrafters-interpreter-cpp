#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "scanner.hpp"

std::string read_file_contents(const std::string& filename);

int main(int argc, char *argv[]) {
    // Disable output buffering
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    if (argc < 3) {
        std::cerr << "Usage: ./your_program tokenize <filename>" << std::endl;
        return 1;
    }

    const std::string command = argv[1];
    std::string file_content = read_file_contents(argv[2]);

    int ret_val = 0;

    if (command == "tokenize") {
        auto result = tokenizer(file_content);
    
        if (result.has_value()) {
            for (const auto& token : result.value()) {
                std::cout << token << std::endl;
            }
        } else {
            std::cerr << "Tokenizer failed with error code: " << result.error() << std::endl;
        }
    } else if (command == "parse"){

        auto result = tokenizer(file_content);
    
        if (result.has_value()) {
            for (const auto& token : result.value()) {
                std::cout << token << std::endl;
            }
        } else {
            std::cerr << "Tokenizer failed with error code: " << result.error() << std::endl;
        }
        
    } else {
        std::cerr << "Unknown command: " << command << std::endl;
        return 1;
    }

    return ret_val;
}

std::string read_file_contents(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error reading file: " << filename << std::endl;
        std::exit(1);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}
