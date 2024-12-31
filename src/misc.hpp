#ifndef AUSTIN_MISC
#define AUSTIN_MISC

#include <string>
#include <vector>
#define AUSTINUTILS __declspec(dllexport)

extern AUSTINUTILS std::vector<std::string> split(std::string s, std::string del = " ");

#endif