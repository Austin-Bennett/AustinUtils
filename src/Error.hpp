#ifndef ERROR_HPP
#define ERROR_HPP
#include <stdexcept>
#define BOOST_STACKTRACE_USE_BACKTRACE
#include <misc.hpp>
#include <boost/stacktrace.hpp>
#include "sstream"
#include "str.hpp"

#define AUSTINUTILS __declspec(dllexport)

namespace AustinUtils {

    class AUSTINUTILS Error : public std::runtime_error {
        protected:
        boost::stacktrace::stacktrace trace;
        mutable std::string s;

        public:
        explicit Error(const str& msg) noexcept;

        [[nodiscard]] const char* what() const noexcept override;
    };



    class Exception : public Error {

        public:
        template<typename... Args>
        explicit Exception(Args... args) : Error(fmt(args...)) {}
    };

    class FileNotFoundError : public Exception {
        public:
        std::string file;

        template<typename... Args>
        explicit FileNotFoundError(std::string file, Args... msg) : Exception("Could not find file ", file, "\n", msg...) {
            this->file = file;
        };
    };
}

#endif
