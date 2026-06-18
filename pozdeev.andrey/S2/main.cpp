#include "expression.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <cstdlib>
#include <utility>

int main(int argc, char *argv[])
{
    std::istream *input = &std::cin;
    std::ifstream fileStream;

    if (argc > 2) {
        std::cerr << "Invalid parameters" << std::endl;
        return 1;
    }

    if (argc == 2) {
        fileStream.open(argv[1]);
        if (!fileStream.is_open()) {
            std::cerr << "Cannot open file: " << argv[1] << std::endl;
            return 1;
        }
        input = &fileStream;
    }

    pozdeev::Stack< long long > results;
    std::string line;

    try {
        while (std::getline(*input, line)) {
            if (line.empty()) {
                continue;
            }

            bool onlySpaces = true;
            for (std::size_t i = 0; i < line.length(); ++i) {
                if (!std::isspace(static_cast< unsigned char >(line[i]))) {
                    onlySpaces = false;
                    break;
                }
            }
            if (onlySpaces) {
                continue;
            }

            pozdeev::List< pozdeev::Token > tokens = pozdeev::tokenizeLine(line);
            if (tokens.isEmpty()) {
                continue;
            }

            pozdeev::Queue< pozdeev::Token > postfix
                = pozdeev::infixToPostfix(tokens);
            long long result = pozdeev::evaluatePostfix(std::move(postfix));
            results.push(result);
        }
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    bool first = true;
    while (!results.isEmpty()) {
        if (!first) {
            std::cout << " ";
        }
        std::cout << results.drop();
        first = false;
    }
    std::cout << std::endl;

    return 0;
}
