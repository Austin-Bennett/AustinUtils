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

#include "Error.hpp"

#define AUSTINUTILS __declspec(dllexport)



using ArgValue = std::any;

using ARG_INT = long long;
using ARG_UINT = unsigned long long;
using ARG_FLOAT = long double;
using ARG_STRING = std::string;
using ARG_BOOL = bool;
using ARG_PATH = std::string;

namespace AustinUtils {

    class ArgumentParsingError : public FormattedError {
        public:
        template<typename... Args>
        explicit ArgumentParsingError(Args... args) : FormattedError(args...) {}
    };

    enum argument_type {
        INT,//aka long long
        UINT,//aka unsigned long long
        FLOAT,//aka long double
        STRING,
        BOOL,//aka, if defined, its true
        FILE_PATH,//like a string, but with path checking (aka, if it exists)
    };

    enum broad_argument_type {
        NUMERICAL,//floats, ints
        UNSIGNED_NUMERICAL,//unsigned ints
        ANY//for anything else (strings, bools, paths)
    };

    extern AUSTINUTILS std::string attos(argument_type typ);

    extern AUSTINUTILS bool is_data_type(argument_type typ, const std::string &data);

    extern AUSTINUTILS broad_argument_type get_broad_type(argument_type typ);

    extern AUSTINUTILS broad_argument_type get_data_type(const std::string& data);


    struct AUSTINUTILS argument {
        std::string key;
        std::string name;
        bool required;
        argument_type type;
        ArgValue data;
    };

    AUSTINUTILS inline bool cast_arg(const argument& arg) {

        if (!arg.data.has_value()) {
            return false;
        }
        try {
            return std::any_cast<bool>(arg.data);
        } catch (const std::bad_any_cast& e) {

            throw ArgumentParsingError(
                "Error, cannot cast argument type of", attos(arg.type), "to type id name of", typeid(bool).name(), "\n");
        }
    }

    template<typename T>
    AUSTINUTILS T cast_arg(const argument& arg) {
        if (!arg.data.has_value()) {

            throw ArgumentParsingError("Argument ", arg.key, " has no value! cannot cast.\n");
        }
        try {
            return std::any_cast<T>(arg.data);
        } catch (const std::bad_any_cast& e) {

            throw ArgumentParsingError(
                "Error, cannot cast argument type of ", attos(arg.type), " to type id name of ", typeid(T).name(), "\n");
        }
    }



    template<typename T>
    AUSTINUTILS T cast_arg_or(argument arg, std::type_identity_t<T> or_else) {
        
        if (!arg.data.has_value()) {
            return or_else;
        }
        try {
            return std::any_cast<T>(arg.data);
        } catch (const std::bad_any_cast& e) {
            throw ArgumentParsingError(
                "Error cannot cast argument type of ", attos(arg.type), " to type id name of ", typeid(T).name(), "\n"
                );
        }
    }





    class AUSTINUTILS argument_parser {
        std::vector<std::string> args;
        int required = 0;
        std::unordered_map<std::string, argument> arg_tokens;
        std::vector<argument> unkeyed_args;
        std::string program_name = "[program_name]";
        public:

        
        argument_parser(int argc, char** argv, int expected_nonrequired = 0);

        argument add_argument(const std::string& name, std::string key, argument_type typ, bool required = true);

        void parse_arguments();

        std::string get_usage();

        argument get_argument(const std::string& name);
    };


}

#endif