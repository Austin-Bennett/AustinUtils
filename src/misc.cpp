#include "misc.hpp"


AUSTINUTILS std::vector<std::string> split(std::string s, std::string del) {
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(del)) != std::string::npos) {
        token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos + del.length());
    }
    tokens.push_back(s);

    return tokens;
}