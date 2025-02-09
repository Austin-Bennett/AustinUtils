#ifndef AUSTIN_UTILS_HPP
#define AUSTIN_UTILS_HPP



#include <string>
#define AUSTINUTILS __declspec(dllexport)
#include <cstdint>


//library files
#ifdef AUSTINUTILS
    #include "math.hpp"
    #include "misc.hpp"
    #include "argparse.hpp"
#endif

namespace AustinUtils {
    extern AUSTINUTILS const std::string AustinUtilsVersion();
}

#define loop while(true)



#endif