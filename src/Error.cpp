#include "Error.hpp"

namespace AustinUtils {
    //Error
    Error::Error(const str &msg)  noexcept : runtime_error(msg.data()) {
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


}