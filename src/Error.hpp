#ifndef ERROR_HPP
#define ERROR_HPP
#include <stdexcept>
#define BOOST_STACKTRACE_USE_BACKTRACE
#include <misc.hpp>
#include <boost/stacktrace.hpp>
#include "sstream"

#define AUSTINUTILS __declspec(dllexport)

namespace AustinUtils {

    class AUSTINUTILS Error : public std::runtime_error {
        protected:
        boost::stacktrace::stacktrace trace;
        mutable std::string s;

        public:
        explicit Error(const std::string& msg) noexcept;

        [[nodiscard]] const char* what() const noexcept override;
    };



    class FormattedError : public Error {

        public:
        template<typename... Args>
        explicit FormattedError(Args... args) : Error(fmt(args...)) {}
    };

    class FileNotFoundError : public FormattedError {
        public:
        template<typename... Args>
        explicit FileNotFoundError(std::string file, Args... msg) : FormattedError("Could not find file ", file, "\n", msg...) {};
    };
}

#endif
