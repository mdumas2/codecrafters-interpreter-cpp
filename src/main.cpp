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

    int ret_val = 0;

    if (command == "tokenize") {
        std::string file_contents = read_file_contents(argv[2]);

        int line_number = 1;
        std::string value;
        size_t start;

        for (size_t i = 0; i < file_contents.size(); ++i) {
            char c = file_contents[i];
            
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
                case '=': 
                    if (i + 1 < file_contents.size() && file_contents[i + 1] == '=') {
                        std::cout << "EQUAL_EQUAL == null" << std::endl;
                        i++;
                    } else {
                        std::cout << "EQUAL = null" << std::endl;
                    }
                    break;
                case '!': 
                    if (i + 1 < file_contents.size() && file_contents[i + 1] == '=') {
                        std::cout << "BANG_EQUAL != null" << std::endl;
                        i++;
                    } else {
                        std::cout << "BANG ! null" << std::endl;
                    }
                    break;
                case '<': 
                    if (i + 1 < file_contents.size() && file_contents[i + 1] == '=') {
                        std::cout << "LESS_EQUAL <= null" << std::endl;
                        i++;
                    } else {
                        std::cout << "LESS < null" << std::endl;
                    }
                    break;
                case '>': 
                    if (i + 1 < file_contents.size() && file_contents[i + 1] == '=') {
                        std::cout << "GREATER_EQUAL >= null" << std::endl;
                        i++;
                    } else {
                        std::cout << "GREATER > null" << std::endl;
                    }
                    break;
                case '/': 
                    if (i + 1 < file_contents.size() && file_contents[i + 1] == '/') {
                        i += 2;
                        while (i < file_contents.size() && file_contents[i] != '\n') {
                            i++;
                        }
                        ++line_number;
                    } else {
                        std::cout << "SLASH / null" << std::endl;
                    }
                    break;
                case '"':
                    start = i;
                    while (i + 1 < file_contents.size() && file_contents[i] != '"') {
                        if (file_contents[i] == '\n') {
                            ++line_number;
                        }
                        i++;
                    }
                    if (i == file_contents.size()) {
                        std::cerr << "[line " << line_number << "] Error: Unterminated string." << c << std::endl; 
                    }
                    value = file_contents.substr(start, i - start - 1);
                    std::cout << "STRING " << value << " " << value << std::endl;
                    break;
                case '\t':
                case '\r':
                case ' ':
                    break;
                case '\n': 
                    ++line_number; 
                    break;
                default: 
                    std::cerr << "[line " << line_number << "] Error: Unexpected character: " << c << std::endl; 
                    ret_val = 65;
                    break;
            }
        }
        
        std::cout << "EOF  null" << std::endl;
        
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
