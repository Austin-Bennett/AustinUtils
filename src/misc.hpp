#ifndef AUSTIN_MISC
#define AUSTIN_MISC

#include <cstdint>
#include <string>
#include <vector>
#include <sstream>


#define AUSTINUTILS __declspec(dllexport)

namespace AustinUtils {

    extern AUSTINUTILS std::vector<std::string> split(std::string s, std::string del = " ");

    template<typename... Args>
    extern AUSTINUTILS std::string fmt(Args... args) {
        std::stringstream format;
        ((format << args), ...);
        return format.str();
    }

    template<typename... Args>
    extern AUSTINUTILS std::wstring wfmt(Args... args) {
        std::wstringstream format;
        ((format << args), ...);
        return format.str();
    }

    template <typename T>
    concept Formattable = requires(std::ostream& os, T value) {
            { os << value } -> std::same_as<std::ostream&>;
    };

    template <typename T>
    concept WideFormattable = requires(std::wostream& os, T value) {
        { os << value } -> std::same_as<std::wostream&>;
    };

}



template<typename T>
std::ostream& operator <<(std::ostream& os, const std::vector<T>& vec) {

    os << std::string("[");

    for (int i = 0; i < vec.size(); i++) {
        if (i != vec.size()-1) {
            os << vec[i] << ",";
        } else {
            os << vec[i];
        }
    }

    os << std::string("]");

    return os;
}

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef size_t usize;

#ifdef WIN64
typedef float f32;
typedef double f64;
typedef long double f80;
#elifdef WIN32
typedef float f32
#endif

#define null nullptr

#endif