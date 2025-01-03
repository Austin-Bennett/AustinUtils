#include "logutils.hpp"
#include <iostream>

std::ofstream logfile = std::ofstream("log.txt");

std::string logger::ltype_to_string(LOGTYPE typ) {
    switch (typ) {

    case INFO:
        return "INFO";
    case WARN:
        return "WARN";
    case ERROR:
        return "ERROR";
    }
    return "";
}

void logger::flush_if_should() {
    if (buffer.empty()) return;
    auto now = high_resolution_clock::now();
    long long ms_dif = duration_cast<milliseconds>(now - flush_time_stamp).count();

    if (ms_dif > 30) {
        flush();
    }
}

logger::logger(std::string identifier) {
    _namespace = identifier;
    flush_time_stamp = high_resolution_clock::now();
    start = high_resolution_clock::now();
    buffer.push_back(log{INFO, ""});
}

high_resolution_clock::time_point logger::get_log_time_start() {
    return start;
}

void logger::flush() {
    flush_time_stamp = high_resolution_clock::now();
    if (buffer.empty()) return;
    for (log Log: buffer) {
        if (Log.message.empty()) continue;
        std::stringstream fmt;
        std::string printout;
        
        fmt << "[" << ltype_to_string(Log.typ) << "][" << _namespace << "]: " << Log.message << "\n";
        printout = fmt.str();
        std::cout << printout;
        logfile << printout;
        
    }
    buffer.clear();
}

logger& logger::operator << (std::string msg) {
    if (buffer.empty()) {
        buffer.push_back({INFO, ""});
    }
    auto tks = split(msg, "\n");
    if (buffer.back().typ != typ) {
        buffer.push_back({typ, ""});
    }
    for (size_t i = 0; i < tks.size(); i++) {
        buffer.back().message += tks[i];
        
        if (i != tks.size()-1) buffer.push_back({typ, ""});
    }

    flush_if_should();

    return *this;
}

logger& logger::operator<<(LOGTYPE typ) {
    this->typ = typ;

    flush_if_should();

    return *this;
}

logger& logger::operator << (char* msg) {

    *this << std::string(msg);



    return *this;
}

logger& logger::operator << (const char* msg) {

    *this << std::string(msg);



    return *this;
}

logger& logger::operator << (char msg) {

    *this << std::string() + msg;


    return *this;
}
logger& logger::operator << (short msg) {
    *this << std::to_string(msg);



    return *this;
}
logger& logger::operator << (int msg) {
    *this << std::to_string(msg);


    return *this;
}
logger& logger::operator << (long msg) {
    *this << std::to_string(msg);


    return *this;
}
logger& logger::operator << (long long msg) {
    *this << std::to_string(msg);


    return *this;
}
logger& logger::operator << (unsigned char msg) {
    *this << std::to_string(msg);


    return *this;
}
logger& logger::operator << (unsigned short msg) {
    *this << std::to_string(msg);


    return *this;
}
logger& logger::operator << (unsigned int msg) {
    *this << std::to_string(msg);


    return *this;
}
logger& logger::operator << (unsigned long msg) {
    *this << std::to_string(msg);



    return *this;
}
logger& logger::operator << (unsigned long long msg) {
    *this << std::to_string(msg);



    return *this;
}
logger& logger::operator << (float msg) {
    *this << std::to_string(msg);



    return *this;
}
logger& logger::operator << (double msg) {
    *this << std::to_string(msg);



    return *this;
}
logger& logger::operator << (long double msg) {
    *this << std::to_string(msg);


    return *this;
}

logger::~logger() {
    flush();
}