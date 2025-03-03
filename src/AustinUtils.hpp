#ifndef AUSTIN_UTILS_HPP
#define AUSTIN_UTILS_HPP



#include <string>
#define AUSTINUTILS __declspec(dllexport)
#include <cstdint>


//library files

#include "math.hpp"
#include "misc.hpp"
#include "Error.hpp"
#include "logging.hpp"
#include "str.hpp"


namespace AustinUtils {
    extern AUSTINUTILS const str AustinUtilsVersion();
}





#endif