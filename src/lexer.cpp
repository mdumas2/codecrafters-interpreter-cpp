#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cctype>

enum class State {
    Start, Identifier, Number, String, Operator, Done
};

class Trie {
private:
    struct Node {
        bool is_end;
        std::unordered_map<char, Node*> children;

        Node() : is_end(false) {}
    };

    Node* root;

public:
    Trie() : root(new Node()) {}

    void insert(const std::string& word) {
        Node* node = root;
        for (char c : word) {
            if (node->children.find(c) == node->children.end()) {
                node->children[c] = new Node();
            }
            node = node->children[c];
        }
        node->is_end = true;
    }

    bool search(const std::string& word) const {
        Node* node = root;
        for (char c : word) {
            if (node->children.find(c) == node->children.end()) {
                return false;
            }
            node = node->children[c];
        }
        return node->is_end;
    }
};

class Lexer {
private:
    std::string file_contents;
    size_t position;
    size_t line_number;

    Trie reservedWordsTrie;
    
    State state;

    char current_char() const {
        if (position >= file_contents.size()) return '\0';
        return file_contents[position];
    }

    void advance() {
        position++;
    }

    bool is_at_end() const {
        return position >= file_contents.size();
    }

    bool is_identifier_char(char c) {
        return std::isalpha(c) || c == '_';
    }

    bool is_digit(char c) {
        return std::isdigit(c);
    }

    void handle_identifier() {
        std::string lexeme;
        while (is_identifier_char(current_char()) || is_digit(current_char())) {
            lexeme += current_char();
            advance();
        }

        // Check if it's a reserved keyword using the Trie
        if (reservedWordsTrie.search(lexeme)) {
            output_token("KEYWORD", lexeme);
        } else {
            output_token("IDENTIFIER", lexeme);
        }
    }

    void handle_number() {
        std::string lexeme;
        while (is_digit(current_char())) {
            lexeme += current_char();
            advance();
        }
        output_token("NUMBER", lexeme);
    }

    void handle_string() {
        std::string lexeme;
        advance();  // Skip the opening quote
        while (current_char() != '"' && !is_at_end()) {
            lexeme += current_char();
            advance();
        }

        if (is_at_end()) {
            throw_error("Unterminated string", "");
        } else {
            advance();  // Skip the closing quote
            output_token("STRING", lexeme);
        }
    }

    void handle_operator() {
        char c = current_char();
        advance();  // Move to the next character after the operator

        switch (c) {
            case '(': output_token("LEFT_PAREN", "("); break;
            case ')': output_token("RIGHT_PAREN", ")"); break;
            case '{': output_token("LEFT_BRACE", "{"); break;
            case '}': output_token("RIGHT_BRACE", "}"); break;
            case '.': output_token("DOT", "."); break;
            case ',': output_token("COMMA", ","); break;
            case ';': output_token("SEMICOLON", ";"); break;
            case '*': output_token("STAR", "*"); break;
            case '-': output_token("MINUS", "-"); break;
            case '+': output_token("PLUS", "+"); break;
            case '=': 
                if (current_char() == '=') {
                    advance(); 
                    output_token("EQUAL_EQUAL", "==");
                } else {
                    output_token("EQUAL", "=");
                }
                break;
            case '!': 
                if (current_char() == '=') {
                    advance(); 
                    output_token("BANG_EQUAL", "!=");
                } else {
                    output_token("BANG", "!");
                }
                break;
            case '<': 
                if (current_char() == '=') {
                    advance(); 
                    output_token("LESS_EQUAL", "<=");
                } else {
                    output_token("LESS", "<");
                }
                break;
            case '>': 
                if (current_char() == '=') {
                    advance(); 
                    output_token("GREATER_EQUAL", ">=");
                } else {
                    output_token("GREATER", ">");
                }
                break;
            default:
                output_token("UNKNOWN_OPERATOR", std::string(1, c));
                break;
        }
    }

    void output_token(const std::string& token_name, const std::string& lexeme, const std::string& additional_info = "null") const {
        std::cout << token_name << " " << lexeme << " " << additional_info << std::endl;
    }

    void throw_error(const std::string& message, const std::string& lexeme) const {
        std::cerr << "[Line " << line_number << "] Error: " << message << " " << lexeme << std::endl;
        exit(1);
    }

public:
    Lexer(const std::string& input) : file_contents(input), position(0), line_number(1), state(State::Start) {
        std::vector<std::string> reservedWords = {"and", "class", "else", "false", "for", "fun", "if", "nil", "or", "print", "return", "super", "this", "true", "var", "while"};
        for (const auto& word : reservedWords) {
            reservedWordsTrie.insert(word);
        }
    }

    void scan_tokens() {
        while (!is_at_end()) {
            char c = current_char();
            switch (state) {
                case State::Start:
                    if (is_identifier_char(c)) {
                        state = State::Identifier;
                        handle_identifier();
                    } else if (is_digit(c)) {
                        state = State::Number;
                        handle_number();
                    } else if (c == '"') {
                        state = State::String;
                        handle_string();
                    } else if (std::ispunct(c)) {
                        state = State::Operator;
                        handle_operator();
                    } else if (c == '\n') {
                        line_number++;
                    } else if (std::isspace(c)) {
                        // Skip whitespace
                    } else {
                        throw_error("Unexpected character", std::string(1, c));
                    }
                    state = State::Start;  // Reset the state
                    advance();
                    break;

                case State::Done:
                    return;
            }
        }
    }
};

//int main() {
//    std::string input = R"(
//        var x = 42;
//        if (x >= 10) {
//            print("x is large");
//        }
//    )";
//
//    Lexer lexer(input);
//    lexer.scan_tokens();
//
//    return 0;
//}