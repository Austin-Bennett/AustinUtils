#include "logging.hpp"

AUSTINUTILS std::string AustinUtils::LogTypeToString(const LOG_TYPE typ) {
    switch (typ) {
        case LOG_INFO:
            return "INFO";
        case LOG_WARN:
            return "WARN";
        case LOG_ERROR:
            return "ERROR";
        case LOG_DEBUG:
            return "DEBUG";
    }
    return "LOG";
}

AUSTINUTILS std::wstring AustinUtils::LogTypeToWString(const LOG_TYPE typ) {
    switch (typ) {
        case AustinUtils::LOG_INFO:
            return L"INFO";
        case AustinUtils::LOG_WARN:
            return L"WARN";
        case AustinUtils::LOG_ERROR:
            return L"ERROR";
        case AustinUtils::LOG_DEBUG:
            return L"DEBUG";
    }
    return L"LOG";
}

AustinUtils::logger::logger(const std::string& name) {
    this->name = name;
}

void AustinUtils::logger::c_log(LOG_TYPE type, const char *fmt, ...) {
    va_list args;
    char buffer[1024];

    va_start(args, fmt);
    vsnprintf(buffer, 1024, fmt, args);
    va_end(args);
    if (type == LOG_ERROR) std::cerr << "[" << LogTypeToString(type) << "][" << name << "]: " << buffer << "\n";
    if (type != LOG_ERROR) std::cout << "[" << LogTypeToString(type) << "][" << name << "]: " << buffer << "\n";

}

AustinUtils::wlogger::wlogger(const std::wstring &name) {
    this->name = name;
}

void AustinUtils::wlogger::c_log(LOG_TYPE type, const wchar_t *fmt, ...) {
    va_list args;
    wchar_t buffer[1024];

    va_start(args, fmt);
    vsnwprintf(buffer, 1024, fmt, args);
    va_end(args);
    if (type == LOG_ERROR) std::wcerr << "[" << LogTypeToWString(type) << "][" << name << "]: " << buffer << "\n";
    if (type != LOG_ERROR) std::wcout << "[" << LogTypeToWString(type) << "][" << name << "]: " << buffer << "\n";
}

