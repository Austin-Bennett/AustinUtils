#include "Error.hpp"

namespace AustinUtils {
    //Error
    Error::Error(const std::string &msg)  noexcept : runtime_error(msg) {
        trace = boost::stacktrace::stacktrace();
        std::stringstream fmt;
        fmt << runtime_error::what() << "\n" << trace << "\n";
        s = fmt.str();
    }

    const char *Error::what() const noexcept {
        std::stringstream fmt;
        fmt << runtime_error::what() << "\n" << trace << "\n";
        s = fmt.str();

        return s.c_str();
    }


    //Formmated Error

}