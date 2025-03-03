#ifndef LOGGING_HPP
#define LOGGING_HPP

#define AUSTINUTILS __declspec(dllexport)
#include <iostream>
#include <misc.hpp>
#include <stdarg.h>
#include <string>

namespace AustinUtils {
    enum LOG_TYPE {
        LOG_INFO,
        LOG_WARN,
        LOG_ERROR,
        LOG_DEBUG
    };

    extern AUSTINUTILS std::string LogTypeToString(LOG_TYPE typ);

    extern AUSTINUTILS std::wstring LogTypeToWString(LOG_TYPE typ);



    class AUSTINUTILS logger {
        protected:
        std::string name;

        public:

        explicit logger(const std::string& name);

        void c_log(LOG_TYPE type, const char *fmt, ...);

        void c_log(LOG_TYPE type, const char *fmt, va_list args);

        template<Formattable... Args>
        void log(LOG_TYPE typ, Args... args) {
            std::string buffer = AustinUtils::fmt(args...);
            if (typ == LOG_ERROR) std::cerr << "[" << LogTypeToString(typ) << "][" << name << "]: " << buffer << "\n";
            if (typ != LOG_ERROR) std::cout << "[" << LogTypeToString(typ) << "][" << name << "]: " << buffer << "\n";

        }

        template<Formattable... Args>
        void info(Args... args) {
            log(LOG_INFO, args...);
        }

        template<Formattable... Args>
        void warn(Args... args) {
            log(LOG_WARN, args...);
        }

        template<Formattable... Args>
        void error(Args... args) {
            log(LOG_ERROR, args...);
        }

        template<Formattable... Args>
        void debug(Args... args) {
            log(LOG_DEBUG, args...);
        }
    };

    class AUSTINUTILS wlogger {
    protected:
        std::wstring name;

    public:

        explicit wlogger(const std::wstring& name);

        void c_log(LOG_TYPE type, const wchar_t *fmt, ...);

        void c_log(LOG_TYPE type, const wchar_t *fmt, va_list args);

        template<WideFormattable... Args>
        void log(LOG_TYPE typ, Args... args) {
            std::wstring buffer = AustinUtils::wfmt(args...);
            if (typ == LOG_ERROR) std::wcerr << "[" << LogTypeToWString(typ) << "][" << name << "]: " << buffer << "\n";
            if (typ != LOG_ERROR) std::wcout << "[" << LogTypeToWString(typ) << "][" << name << "]: " << buffer << "\n";

        }

        template<WideFormattable... Args>
        void info(Args... args) {
            log(LOG_INFO, args...);
        }

        template<WideFormattable... Args>
        void warn(Args... args) {
            log(LOG_WARN, args...);
        }

        template<WideFormattable... Args>
        void error(Args... args) {
            log(LOG_ERROR, args...);
        }

        template<WideFormattable... Args>
        void debug(Args... args) {
            log(LOG_DEBUG, args...);
        }
    };

}

#endif
