#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

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

    if (command == "tokenize") {
        std::string file_contents = read_file_contents(argv[2]);
        int line_number = 1; // Start from line 1
        for (auto c: file_contents) {
            switch (c) {
                case '(': std::cout << "LEFT_PAREN ( null" << std::endl; break;
                case ')': std::cout << "RIGHT_PAREN ) null" << std::endl; break;
                case '{': std::cout << "LEFT_BRACE { null" << std::endl; break;
                case '}': std::cout << "RIGHT_BRACE } null" << std::endl; break;
                case '.': std::cout << "DOT . null" << std::endl; break;
                case ',': std::cout << "COMMA , null" << std::endl; break;
                case ';': std::cout << "SEMICOLON ; null" << std::endl; break;
                case '*': std::cout << "STAR * null" << std::endl; break;
                case '-': std::cout << "MINUS - null" << std::endl; break;
                case '+': std::cout << "PLUS + null" << std::endl; break;
                case '\n': line_number++; break;  // Increment line number on each newline
                default: 
                    std::cerr << "[line " << line_number << "] Error: Unexpected character: " << c << std::endl; 
                    return 65;
            }
        }
        
        std::cout << "EOF  null" << std::endl;
        
    } else {
        std::cerr << "Unknown command: " << command << std::endl;
        return 1;
    }

    return 0;
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
