#include "argparse.hpp"
#include <cstring>
#include <iostream>
#include <sstream>
#include <cctype>
#include <fstream>
#include <misc.hpp>
#include <utility>
#include <algorithm>



AUSTINUTILS std::string AustinUtils::attos(argument_type typ) {
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




AUSTINUTILS bool AustinUtils::is_data_type(argument_type typ, const std::string &data) {
    if (data.empty()) return false;
    bool is_numerical = true;
    bool can_be_unsigned = true;

    for (size_t i = 0; i < data.length(); i++) {
        if (!std::isdigit(data[i]) && data[i] != '.' && (i == 0 && data[i] != '-')) {
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

AUSTINUTILS ArgValue string_to_type(std::string s, AustinUtils::argument_type typ) {
    switch (typ) {
        case AustinUtils::INT:
            return ArgValue(std::stoll(s));
        case AustinUtils::UINT:
            return ArgValue(std::stoull(s));
        case AustinUtils::FLOAT:
            return ArgValue(std::stold(s));
        case AustinUtils::STRING:
            return s;
        case AustinUtils::FILE_PATH:
            return s;
        case AustinUtils::BOOL:
            return ArgValue(true);
        }
    return ArgValue(true);
}

AUSTINUTILS AustinUtils::broad_argument_type AustinUtils::get_broad_type(const argument_type typ) {
    switch (typ) {
        case INT:
            return NUMERICAL;
        case UINT:
            return UNSIGNED_NUMERICAL;
        case FLOAT:
            return NUMERICAL;
        default:
            return ANY;
    }
}


AUSTINUTILS AustinUtils::broad_argument_type AustinUtils::get_data_type(const std::string& data) {
    if (data.empty()) return ANY;
    bool is_numerical = true;

    bool can_be_unsigned = true;

    for (size_t i = 0; i < data.length(); i++) {
        if (!std::isdigit(data[i]) && data[i] != '.' && data[i] != '-') {
            is_numerical = false;
        }
        if (data[i] == '-') {
            can_be_unsigned = false;
        }
    }

    if (is_numerical && !can_be_unsigned) return NUMERICAL;
    if (is_numerical) return UNSIGNED_NUMERICAL;
    return ANY;
}


//CLASS ARGUMENT PARSER

AustinUtils::argument_parser::argument_parser(int argc, char** argv, int expected_nonrequired) {
    required = expected_nonrequired;
    for (int i = 1; i < argc; i++) {
        this->args.emplace_back(argv[i]);
    }
    this->program_name = std::string(argv[0]);

    if (usize optimize = 0; (optimize = program_name.rfind('/')) != std::string::npos) {
        program_name = program_name.substr(optimize+1);
    } else if ((optimize = program_name.rfind('\\')) != std::string::npos) {
        program_name = program_name.substr(optimize+1);
    }
}

AustinUtils::argument AustinUtils::argument_parser::add_argument(const std::string& name, std::string key, argument_type typ, bool required) {
    if (key.empty()) {
        if (typ == BOOL) {
            throw ArgumentParsingError("Cannot create bool with unkeyed argument");
        }
        if (required) this->required ++;
        unkeyed_args.emplace_back(argument{"", name, required, typ, ArgValue()});
        return unkeyed_args.back();
    }
    
    if (arg_tokens.contains(name)) {
        throw ArgumentParsingError("Cannot create 2 arguments of the same name: ", name);
    }
    arg_tokens[name] = {std::move(key), name, required, typ, ArgValue()};
    if (required && typ != BOOL) this->required ++;
    return arg_tokens[name];
    
}

std::string AustinUtils::argument_parser::get_usage() {
    std::stringstream ret;
    ret << program_name << " ";
    for (const auto&[name, arg]: arg_tokens) {
        if (arg.required && arg.type != BOOL) {
            
            ret << arg.key << " [" << name << "] ";
            
        }
    }
    for (const auto& arg: unkeyed_args) {
        if (arg.required && arg.type != BOOL) {
            ret << "[" << arg.name << "]";
        }
    }
    for (const auto&[name, arg]: arg_tokens) {
        if (!arg.required || arg.type == BOOL) {
        
            if (arg.type != BOOL) {
                ret << "[" << arg.key << " [" << name << "]] ";
            } else {
                ret << "[" << arg.key << "] ";
            }

        }
    }

    for (const auto& arg: unkeyed_args) {
        if (!arg.required) {
            ret << "[[" << arg.name << "]]";
        }
    }


    return ret.str();
}

void AustinUtils::argument_parser::parse_arguments() {
    int req_args = required;
    for (auto& [name, arg]: arg_tokens) {
        if (arg.key.empty()) {
            throw ArgumentParsingError(
                "Error parsing arguments\n", name, " has no key, could not parse.\n", "Usage: ", get_usage());

        }
        for (usize i = 0; i < args.size(); i++) {
            if (args[i] == arg.key) {
                if (arg.type != BOOL && i == args.size()-1) {
                    throw ArgumentParsingError(
                        "Error expected type ", attos(arg.type), " for argument ", name, " [", arg.key, "] \n", "Usage: ", get_usage());
                }
                if (arg.type != BOOL && arg.type != FILE_PATH) {
                    if (!is_data_type(arg.type, args[i+1])) {
                        throw ArgumentParsingError(
                        "Error expected type ", attos(arg.type), " for argument ", name, " [", arg.key, "] \n", "Usage: ", get_usage());
                    }
                    arg.data = string_to_type(args[i+1], arg.type);
                }
                if (arg.type == BOOL) {
                    arg.data = true;
                    args.erase(args.begin()+static_cast<i64>(i));
                    break;
                }
                if (arg.type == FILE_PATH) {
                    if (!is_data_type(arg.type, args[i+1])) {
                        throw ArgumentParsingError(
                        "Error expected type ", attos(arg.type), " for argument ", name, " [", arg.key, "] \n", "Usage: ", get_usage());
                    }
                    std::fstream f(args[i+1]);
                    if (!f.is_open()) {
                        throw FileNotFoundError(args[i+1], "For argument ", name, "[", arg.key, "]", "\nUsage: ", get_usage());
                    }
                    f.close();
                    arg.data = string_to_type(args[i+1], arg.type);
                }
                req_args--;
                args.erase(args.begin()+static_cast<i64>(i+1));
                args.erase(args.begin()+static_cast<i64>(i));
                break;
            }
        }
    }

    for (auto& arg: unkeyed_args) {
        //simply look for the first thing of the type
        for (usize i = 0; i < args.size(); i++) {
            if (get_data_type(args[i]) == get_broad_type(arg.type)) {
                arg.data = string_to_type(args[i], arg.type);
                if (arg.type == FILE_PATH) {
                    std::fstream f(args[i]);
                    if (!f.is_open()) {
                        throw FileNotFoundError(args[i+1], "For argument ", arg.name, "\nUsage: ", get_usage());
                    }
                    f.close();
                }
                req_args--;
                args.erase(args.begin()+static_cast<i64>(i));
                break;
            }
        }
    }

    if (req_args > 0) {
        throw ArgumentParsingError("Error, expected ", required, " arguments, got ", required-req_args, "\nUsage: ", get_usage());
    }
}

AustinUtils::argument AustinUtils::argument_parser::get_argument(const std::string& name) {
    if (arg_tokens.contains(name)) return arg_tokens[name];
    auto it = std::find_if(unkeyed_args.begin(), unkeyed_args.end(),
        [&name](const argument& arg) {
        return arg.name == name;
    });

    if (it != unkeyed_args.end()) return *it;
    throw ArgumentParsingError("No argument named: ", name);
}