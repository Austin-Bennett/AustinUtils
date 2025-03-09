#ifndef AUSTIN_MISC
#define AUSTIN_MISC

#include <cstdint>
#include <fcntl.h>
#include <string>
#include <vector>
#include <sstream>
#include <io.h>
#include <windows.h>
#include <limits>

#define AUSTINUTILS __declspec(dllexport)

namespace AustinUtils {
    class str;

    extern AUSTINUTILS std::vector<std::string> split(std::string s, std::string del = " ");

    template<typename T>
    concept Iterable = requires(T t) {
        {t.begin()} -> std::same_as<decltype(t.begin())>;
        {t.end()} -> std::same_as<decltype(t.end())>;
    };

    template <typename T>
    concept Formattable = requires(std::ostream& os, T value) {
        { os << value } -> std::same_as<std::ostream&>;
    };

    template <typename T>
    concept WideFormattable = requires(std::wostream& os, T value) {
        { os << value } -> std::same_as<std::wostream&>;
    };

    template<Formattable... Args>
    extern AUSTINUTILS std::string fmt(Args... args) {
        std::stringstream format;
        ((format << args), ...);
        return format.str();
    }

    template<WideFormattable... Args>
    extern AUSTINUTILS std::wstring wfmt(Args... args) {
        std::wstringstream format;
        ((format << args), ...);
        return format.str();
    }


    template<Formattable T>
    std::ostream& operator <<(std::ostream& os, const std::vector<T>& vec) {

        os << "[";

        auto it = vec.begin();

        while (it != vec.end()) {
            os << *it;
            if (++it != vec.end()) os << ", ";
        }

        os << "]";

        return os;
    }

    const static UINT og_console_CP = GetConsoleCP();

    inline void SetConsoleUTFMode(const bool value) {
        if (value) {
            _setmode(_fileno(stdout), _O_U16TEXT);
            SetConsoleOutputCP(CP_UTF8);
        } else {
            _setmode(_fileno(stdout), _O_TEXT);
            SetConsoleOutputCP(og_console_CP);
        }
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

    typedef wchar_t wchar;

#ifdef WIN64
    typedef float f32;
    typedef double f64;
    typedef long double f80;
#elifdef WIN32
    typedef float f32
#endif

    #define null nullptr

    //cast macro for all those static casts
    #define cast(x, T) (static_cast<T>(x))

    //NODISCARD is just a macro wrapper for [[nodiscard]]
    #define NODISCARD [[nodiscard]]

    //returns the maximum value of a type T
    #define T_MAX(T) (std::numeric_limits<T>::max())
    #define T_MIN(T) (std::numeric_limits<T>::lowest())

    //i like this rust keyword
    #define loop while(true)

    template<typename T>
    class basic_random_access_iterator {
    protected:
        T* ptr;

    public:

        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        explicit basic_random_access_iterator(T* ptr) : ptr(ptr) {}

        T& operator *() {
            return *ptr;
        }

        T* operator ->() {
            return ptr;
        }

        T& operator [](usize index) {
            return ptr[index];
        }

        basic_random_access_iterator operator ++(int) {
            basic_random_access_iterator ret = *this;
            ++ptr;
            return ret;
        }

        basic_random_access_iterator& operator ++() {
            ++ptr;
            return *this;
        }

        basic_random_access_iterator operator --(int) {
            basic_random_access_iterator ret = *this;
            --ptr;
            return ret;
        }

        basic_random_access_iterator& operator --() {
            --ptr;
            return *this;
        }

        basic_random_access_iterator operator +(std::ptrdiff_t amt) {
            return basic_random_access_iterator(ptr+amt);
        }

        basic_random_access_iterator operator -(std::ptrdiff_t amt) {
            return basic_random_access_iterator(ptr-amt);
        }

        basic_random_access_iterator& operator +=(std::ptrdiff_t amt) {
            ptr += amt;
            return *this;
        }

        basic_random_access_iterator& operator -=(std::ptrdiff_t amt) {
            ptr -= amt;
            return *this;
        }

        bool operator ==(const basic_random_access_iterator &o) const {
            return ptr == o.ptr;
        }

        bool operator !=(const basic_random_access_iterator &o) const {
            return ptr != o.ptr;
        }

        bool operator >(const basic_random_access_iterator &o) const {
            return ptr > o.ptr;
        }

        bool operator >=(const basic_random_access_iterator &o) const {
            return ptr >= o.ptr;
        }

        bool operator <(const basic_random_access_iterator &o) const {
            return ptr < o.ptr;
        }

        bool operator <=(const basic_random_access_iterator &o) const {
            return ptr <= o.ptr;
        }

    };

    template<typename T>
    class basic_iterator {
    protected:
        T* ptr;

    public:

        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        explicit basic_iterator(T* ptr) : ptr(ptr) {}

        T& operator *() {
            return *ptr;
        }

        T* operator ->() {
            return ptr;
        }

        basic_iterator operator ++(int) {
            basic_iterator ret = *this;
            ++ptr;
            return ret;
        }

        basic_iterator& operator ++() {
            ++ptr;
            return *this;
        }

        basic_iterator operator --(int) {
            basic_iterator ret = *this;
            --ptr;
            return ret;
        }

        basic_iterator& operator --() {
            --ptr;
            return *this;
        }

        basic_iterator operator +(std::ptrdiff_t amt) {
            return basic_iterator(ptr+amt);
        }

        basic_iterator operator -(std::ptrdiff_t amt) {
            return basic_iterator(ptr-amt);
        }

        basic_iterator& operator +=(std::ptrdiff_t amt) {
            ptr += amt;
            return *this;
        }

        basic_iterator& operator -=(std::ptrdiff_t amt) {
            ptr -= amt;
            return *this;
        }

        bool operator ==(const basic_iterator &o) const {
            return ptr == o.ptr;
        }

        bool operator !=(const basic_iterator &o) const {
            return ptr != o.ptr;
        }

        bool operator >(const basic_iterator &o) const {
            return ptr > o.ptr;
        }

        bool operator >=(const basic_iterator &o) const {
            return ptr >= o.ptr;
        }

        bool operator <(const basic_iterator &o) const {
            return ptr < o.ptr;
        }

        bool operator <=(const basic_iterator &o) const {
            return ptr <= o.ptr;
        }

    };

    template<typename T>
    class basic_reverse_iterator {
    protected:
        T* ptr;

    public:

        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        explicit basic_reverse_iterator(T* ptr) : ptr(ptr) {}

        T& operator *() {
            return *ptr;
        }

        T* operator ->() {
            return ptr;
        }

        basic_reverse_iterator operator ++(int) {
            basic_reverse_iterator ret = *this;
            --ptr;
            return ret;
        }

        basic_reverse_iterator& operator ++() {
            --ptr;
            return *this;
        }

        basic_reverse_iterator operator --(int) {
            basic_reverse_iterator ret = *this;
            ++ptr;
            return ret;
        }

        basic_reverse_iterator& operator --() {
            ++ptr;
            return *this;
        }

        basic_reverse_iterator operator +(std::ptrdiff_t amt) {
            return basic_reverse_iterator(ptr-amt);
        }

        basic_reverse_iterator operator -(std::ptrdiff_t amt) {
            return basic_reverse_iterator(ptr+amt);
        }

        basic_reverse_iterator& operator +=(std::ptrdiff_t amt) {
            ptr -= amt;
            return *this;
        }

        basic_reverse_iterator& operator -=(std::ptrdiff_t amt) {
            ptr += amt;
            return *this;
        }

        bool operator ==(const basic_reverse_iterator &o) const {
            return ptr == o.ptr;
        }

        bool operator !=(const basic_reverse_iterator &o) const {
            return ptr != o.ptr;
        }

        bool operator >(const basic_reverse_iterator &o) const {
            return ptr < o.ptr;
        }

        bool operator >=(const basic_reverse_iterator &o) const {
            return ptr <= o.ptr;
        }

        bool operator <(const basic_reverse_iterator &o) const {
            return ptr > o.ptr;
        }

        bool operator <=(const basic_reverse_iterator &o) const {
            return ptr >= o.ptr;
        }

    };



}
#endif