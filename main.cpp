// Include necessary libraries
#include <iostream> // Input/output stream
#include <string> // String operations
#include <utility>
#include <vector> // Vector container
#include <regex> // Regular expression operations
#include <unordered_set> // Unordered set container
#include <fstream>

using namespace std;

// Struct for token representation
struct Token {
    string name;
    string pattern_string;
    regex pattern;
    // Constructor for Token struct
    Token(string name, const string& pattern_string) :
            name(std::move(name)), pattern_string(pattern_string), pattern(pattern_string)
    {}
};

// Function that performs lexical analysis (tokenization) on a given string
vector<pair<string, string>> lexer(string);

// Main function
int main() {
    // Read input from file
    ifstream file("C:\\Users\\isaia\\CLionProjects\\Project-Burnt\\input.txt");
    if (!file) {
        cerr << "Failed to open file\n";
        return 1;
    }

    string str((istreambuf_iterator<char>(file)), istreambuf_iterator<char>()); // read the file into a string

    // Perform lexical analysis on input string
    vector<pair<string, string>> tokens = lexer(str);

    // Print out the tokens
    for (const auto& token : tokens) {
        cout << token.first << ": " << token.second << endl;
    }

    // Return 0 to indicate successful execution
    return 0;
}

// Definition of lexer function that performs lexical analysis on a given string
vector<pair<string, string>> lexer(string given_string)
{
    // Define token types
    Token t[] = {
            {"integer", "[0-9]+"},
            {"double", "[0-9]+\\.[0-9]*"},
            {"string", R"("((\\")|[^\\"]|\s)*")"},
            {"identifier", "[a-zA-Z_][a-zA-Z0-9_]*"},
            {"plus", "\\+"},
            {"minus", "\\-"},
            {"multiply", "\\*"},
            {"divide", "\\/"},
            {"equals", "\\="},
            {"whitespace", "\\s+"},
            {"newline", "\n"},
    };

    // Convert token types to vector for easier manipulation
    vector<Token> token_types(begin(t), end(t));

    // Set of all variable names
    unordered_set<string> variable_names;

    // Tokenize the input using regular expressions
    vector<pair<string, string>> tokens; // Flag to indicate if the token matches any token type
    regex whitespace("[ \t]+");
    sregex_token_iterator it(given_string.begin(), given_string.end(), whitespace, -1);
    sregex_token_iterator end;
    for (; it != end; ++it) {
        string token = it->str();
        bool matched = false; // Extract token from iterator
        for (const auto& token_type : token_types) { // Iterate over all token types
            if (regex_match(token, token_type.pattern)) { // Check if token matches the current token type pattern
                matched = true; // Set matched flag to true
                if (token_type.name == "identifier") { // If token is an identifier
                    if (variable_names.find(token) != variable_names.end()) { // Check if identifier is already declared
                        tokens.emplace_back("variable", token); // Add token as variable usage
                    } else {
                        variable_names.insert(token); // Add identifier to set of declared variable names
                        tokens.emplace_back("declaration", token); // Add token as variable declaration

                    }
                } else {
                    tokens.emplace_back(token_type.name, token); // Add token and token type name to output vector
                }
                break; // Break out of loop after adding token to output vector
            }
        }
        if (!matched) { // If token does not match any token type
            tokens.emplace_back("unknown", token);
        }
    }
    return tokens;
}