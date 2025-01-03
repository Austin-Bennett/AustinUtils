#include "argparse.hpp"
#include <cstring>
#include <iostream>
#include <sstream>
#include <cctype>
#include <fstream>


AUSTINUTILS std::string attos(argument_type typ) {
    switch (typ) {
    case INT:
        return "integer";
    case UINT:
        return "unsigned integer";
    case FLOAT:
        return "float";
    case STRING:
        return "string";
    case BOOL:
        return "bool";
    case FILE_PATH:
        return "file path";
    }
    return "";
}




AUSTINUTILS bool is_data_type(argument_type typ, std::string data) {
    if (data.length() == 0) return false;
    bool is_numerical = 1;
    bool can_be_unsigned = 1;

    for (size_t i = 0; i < data.length(); i++) {
        if (!std::isdigit(data[i]) && data[i] != '.' && data[i] != '-') {
            is_numerical = false;
        }
        if (data[i] == '-') {
            can_be_unsigned = false;
        }
    }


    switch (typ) {
        case INT:
            return is_numerical;
        case UINT:
            return is_numerical && can_be_unsigned;
        case FLOAT:
            return is_numerical;
        case STRING:
            return true;
        case BOOL:
            return true;
        case FILE_PATH:
            return true;
    }
    return false;
}

AUSTINUTILS ArgValue string_to_type(std::string s, argument_type typ) {
    switch (typ) {
        case INT:
            return ArgValue(std::stoll(s));
        case UINT:
            return ArgValue(std::stoull(s));
        case FLOAT:
            return ArgValue(std::stold(s));
        case STRING:
            return s;
        case FILE_PATH:
            return s;
        case BOOL:
            return ArgValue(true);
        }
    return ArgValue(true);
}


//CLASS ARGUMENT PARSER

argument_parser::argument_parser(int argc, char** argv, int expected_nonrequired) {
    required = expected_nonrequired;
    for (int i = 1; i < argc; i++) {
        this->args.push_back(argv[i]);
    }
    this->program_name = argv[0];
}

argument argument_parser::add_argument(std::string name, std::string key, argument_type typ, bool required) {

    
    if (arg_tokens.find(name) != arg_tokens.end()) {
        std::cerr << "Cannot create 2 arguments of the same name: " << name << "\n";
        abort();
    }
    arg_tokens[name] = {key, required, typ, ArgValue()};
    if (required && typ != BOOL) this->required ++;
    return arg_tokens[name];
    
}

std::string argument_parser::get_usage() {
    std::stringstream ret;
    ret << program_name << " ";
    for (auto[name, arg]: arg_tokens) {
        if (arg.required && arg.type != BOOL) {
            
            ret << arg.key << " [" << name << "] ";
            
        }
    }
    for (auto[name, arg]: arg_tokens) {
        if (!arg.required || arg.type == BOOL) {
        
            if (arg.type != BOOL) {
                ret << "[" << arg.key << " [" << name << "]] ";
            } else {
                ret << "[" << arg.key << "] ";
            }

        }

        
    }


    return ret.str();
}

void argument_parser::parse_arguments() {
    int req_args = required;
    for (auto& [name, arg]: arg_tokens) {
        
        if (arg.key == "") {
            std::cerr << "Error parsing arguments\n";
            std::cerr << name << " has no key, could not parse.\n";
            std::cerr << "Usage: " << get_usage() << "\n";
            abort();
        }
        for (size_t i = 0; i < args.size(); i++) {
            if (args[i] == arg.key) {
                if (arg.type != BOOL && i == args.size()-1) {
                    std::cerr << "Error expected " << attos(arg.type) << " for argument " << name << " [" << arg.key << "] " << "\n";
                    std::cerr << "Usage: " << get_usage() << "\n";
                    abort();
                }
                if (arg.type != BOOL && arg.type != FILE_PATH) {
                    if (!is_data_type(arg.type, args[i+1])) {
                        std::cerr << "Error, expected type " << attos(arg.type) << " for argument " << name << " [" << arg.key << "] " << "\n";
                        std::cerr << "Usage: " << get_usage() << "\n";
                        abort();
                    }
                    arg.data = string_to_type(args[i+1].c_str(), arg.type);
                }
                if (arg.type == BOOL) {
                    arg.data = true;
                    
                }
                if (arg.type == FILE_PATH) {
                    if (!is_data_type(arg.type, args[i+1])) {
                        std::cerr << "Error, expected type " << attos(arg.type) << " for argument " << name << " [" << arg.key << "] " << "\n";
                        std::cerr << "Usage: " << get_usage() << "\n";
                        abort();
                    }
                    std::fstream f(args[i+1]);
                    if (!f.is_open()) {
                        std::cerr << "Error, could not find file " << args[i+1] << " for argument " << name << " [" << arg.key << "] " << "\n";
                        std::cerr << "Usage: " << get_usage() << "\n";
                        abort();
                    }
                    f.close();
                    arg.data = string_to_type(args[i+1].c_str(), arg.type);
                }
                req_args--;
            }
        }
    }

    if (req_args > 0) {
        std::cerr << "Error, expected " << required << " arguments, got " << required-req_args << "\n";
        std::cerr << "Usage: " << get_usage() << "\n";
        abort();
    }
}

argument argument_parser::get_argument(std::string name) {
    return arg_tokens[name];
}