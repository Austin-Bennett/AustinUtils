#ifndef AUSTINUTILS_ARGPARSE
#define AUSTINUTILS_ARGPARSE

#include <any>
#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#define AUSTINUTILS __declspec(dllexport)

enum argument_type {
    INT,//aka long long
    UINT,//aka unsigned long long
    FLOAT,//aka long double
    STRING,
    BOOL,//aka, if defined, its true
    FILE_PATH,//like a string, but with path checking (aka, if it exists)
};

extern AUSTINUTILS std::string attos(argument_type typ);

using ArgValue = std::any;
using ARG_INT = long long;
using ARG_UINT = unsigned long long;
using ARG_FLOAT = long double;
using ARG_STRING = std::string;
using ARG_BOOL = bool;
using ARG_PATH = std::string;


extern AUSTINUTILS void set_data(argument_type typ, std::string s, void* data);

extern AUSTINUTILS bool is_data_type(argument_type typ, std::string data);



struct AUSTINUTILS argument {
    std::string key;
    bool required;
    argument_type type;
    ArgValue data;
};


template<typename T>
extern AUSTINUTILS T cast_arg(argument arg) {

    if (!arg.data.has_value()) {
        std::cerr << "argument" << arg.key << "has no value! cannot cast.\n";
        abort();
    }
    try {
        return std::any_cast<T>(arg.data);
    } catch (const std::bad_any_cast& e) {
        std::cerr << "Error, cannot cast argument type of " << attos(arg.type) << " to type id name of " << typeid(T).name() << "\n";
        abort();
    }
}

template<typename T>
extern AUSTINUTILS T cast_arg_or(argument arg, std::type_identity_t<T> or_else) {

    if (!arg.data.has_value()) {
        return or_else;
    }
    try {
        return std::any_cast<T>(arg.data);
    } catch (const std::bad_any_cast& e) {
        std::cerr << "Error, cannot cast argument type of " << attos(arg.type) << " to type id name of " << typeid(T).name() << "\n";
        abort();
    }
}

class AUSTINUTILS argument_parser {
    std::vector<std::string> args;
    int required = 0;
    std::unordered_map<std::string, argument> arg_tokens;
    std::string program_name = "[program_name]";
    public:

    
    argument_parser(int argc, char** argv, int expected_nonrequired = 0);

    argument add_argument(std::string name, std::string key, argument_type typ, bool required = true);

    void parse_arguments();

    std::string get_usage();

    argument get_argument(std::string name);
};



#endif