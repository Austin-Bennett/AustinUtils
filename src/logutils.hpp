#ifndef AUSTIN_LOG_UTILS_HPP
#define AUSTIN_LOG_UTILS_HPP

#include <fstream>
#include <chrono>
#include "misc.hpp"
using namespace std::chrono;

#ifdef ERROR
#undef ERROR
#endif

extern std::ofstream logfile;

#define AUSTINUTILS __declspec(dllexport)




class AUSTINUTILS logger {
    public:
    enum LOGTYPE {
        INFO,
        WARN,
        ERROR
    };

    struct log {
        LOGTYPE typ;
        std::string message;
    };

    protected:

    std::string ltype_to_string(LOGTYPE lt);


    LOGTYPE typ = INFO;
    std::string _namespace;
    std::vector<log> buffer;
    high_resolution_clock::time_point start;
    std::chrono::time_point<high_resolution_clock> flush_time_stamp;

    void flush_if_should();

    public:

    logger(std::string identifier);

    //flush the buffered logs
    void flush();

    high_resolution_clock::time_point get_log_time_start();

    logger& operator << (std::string msg);
    logger& operator << (LOGTYPE typ);
    logger& operator << (char* msg);
    logger& operator << (const char* msg);
    logger& operator << (char msg);
    logger& operator << (short msg);
    logger& operator << (int msg);
    logger& operator << (long msg);
    logger& operator << (long long msg);
    logger& operator << (unsigned char msg);
    logger& operator << (unsigned short msg);
    logger& operator << (unsigned int msg);
    logger& operator << (unsigned long msg);
    logger& operator << (unsigned long long msg);
    logger& operator << (float msg);
    logger& operator << (double msg);
    logger& operator << (long double msg);

    template<typename T>
    friend logger& operator <<(logger& lg, T obj);

    ~logger();
};

#endif