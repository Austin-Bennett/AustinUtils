#ifndef STR_HPP
#define STR_HPP
#include <misc.hpp>
#include "math.hpp"
#include <cstring>
#include <iomanip>


//create a better string than the c++ string, with support for java-like string stuff



namespace AustinUtils {

    class str;
    template<typename T>
    concept Stringifieable = requires(T t)
    {
        {t.toStr()} -> std::same_as<str>;
    };



    class AUSTINUTILS str {
    private:
        char* cstr = null;
        usize slength;
        usize msize;

        void dealloc();

        void simple_dealloc();

    public:

        class iterator : public basic_iterator<char> {
            public:
            explicit iterator(char* ptr) : basic_iterator(ptr) {}
        };

        class reverse_iterator : public basic_reverse_iterator<char> {
        public:
            explicit reverse_iterator(char* ptr) : basic_reverse_iterator(ptr) {}
        };




        static constexpr usize npos = T_MAX(usize);

        virtual ~str();


        using value_type = char;
        using reference = char&;
        using const_reference = const char&;

        /*
         * Creates string of 0 length
         */
        str();

        /*
         * creates a string from a char array or from a string literal
         */
        str(const char* c_str);

        /*
         * creates a string from an existing string from start to start+min(len, s.len())
         */
        str(const str& s, usize start, usize len = npos);

        /*
         * creates a string from the first n chars of a char array or string literal
         */
        str(const char* c_str, usize n);

        //creates a string from a begin and end iterator
        template<class iterator>
        str(iterator begin, iterator end) {
            iterator cur = begin;
            slength = 0;
            while (cur != end) {
                slength++;
                ++cur;
            }


            msize = slength+5;
            cstr = new char[msize];

            for (usize i = 0; begin != end; ++begin) {
                cstr[i] = static_cast<char>(*begin);
                i++;
            }
            cstr[slength] = '\0';
        }


        //creates a string from an initializer_list of chars
        str(std::initializer_list<char> il);

        /*
         * creates a string of 'count' number of chars
         */
        str(char c, usize count);

        /*
         * creates a string from a standard-library string
         */
        str(const std::string& s);

        /*
         * creates a string by copying s
         */
        str(const str& s);

        template<typename T, typename = std::enable_if_t<!std::is_floating_point_v<T>>>
        explicit str(T x) {
            str s;
            s.append(x);

            slength = s.slength;
            msize = s.msize;
            cstr = s.cstr;
            s.cstr = nullptr;
            s.slength = 0;
            s.msize = 0;
        }

        template<FloatingPoint T>
        explicit str(T x, usize precision = 6) {
            str s;
            s.append(x, precision);

            slength = s.slength;
            msize = s.msize;
            cstr = s.cstr;
            s.cstr = nullptr;
            s.slength = 0;
            s.msize = 0;
        }

        /*
         * creates a string by moving s
         */
        str(str&& s) noexcept;

        template<typename T>
        str& assign(T x) {
            return this->operator=(str(x));
        }

        str& assign(const str& s);

        str& assign(const str&& s);

        str& assign(const str& s, usize start, usize len = npos);

        str& assign(const char* c_str);

        str& assign(const char* c_str, usize n);

        str& assign(char c, usize n);

        template<class iterator>
        str& assign(iterator begin, iterator end) {
            return this->operator=(str(begin, end));
        }

        str& assign(std::initializer_list<char> il);



        str& operator =(const str& other);

        str& operator =(str&& other) noexcept ;

        NODISCARD usize len() const;

        NODISCARD usize capacity() const;

        void resize(usize new_size);

        void clear();

        NODISCARD bool empty() const;

        void shrink_to_fit();

        NODISCARD char& at(const usize index) const {
            if (index > msize) throw std::out_of_range("Cannot access element at " + std::to_string(index));
            return cstr[index];
        }

        NODISCARD char& operator [](const usize index) const {
            return at(index);
        }

        char& back() const {
            if (slength == 0) throw std::out_of_range("Cannot access element at the back of an empty string");
            return cstr[slength-1];
        }

        char& front() const {
            if (slength == 0) throw std::out_of_range("Cannot access element at the front of an empty string");
            return cstr[0];
        }

        friend std::ostream& operator<<(std::ostream& os, const str& self) {
            os << self.cstr;
            return os;
        }

        friend std::istream& operator>>(std::istream& is, str& s) {
            s.clear();
            char c;

            if(is.get(c)){
                s.append(c);
                while((is.get(c)) && (c != ' '))
                    s.append(c);
                if(!is.bad() && is.eof())
                    is.clear(std::ios_base::eofbit);
            }
            return is;
        }

        void append(const str& s);

        void append(const char* s);

        void append(const std::string& s);

        void append(char c);

        template<Integral T>
        void append(T x) {
            append(std::to_string(x));
        }


        void append(void* x) {
            if (x == nullptr) {
                append("null");
                return;
            }
            usize needed = snprintf(null, 0, "%p", x);

            auto buffer = new char[needed+1];
            snprintf(buffer, needed+1, "%p", x);
            append("0x");
            append(buffer);
            delete[] buffer;
            return;
        }

        template<FloatingPoint T>
        void append(T x, usize precision = 6) {
            std::ostringstream stream;
            stream << std::fixed << std::setprecision(precision) << x;
            append(stream.str());
        }

        template<Stringifieable T>
        void append(T& x) {
            append(x.toStr());
        }

        template<typename T>
        str& operator +=(T x) {
            append(x);
            return *this;
        }


        template<typename T>
        str operator +(T x) {
            str ret = *this;
            ret.append(x);
            return ret;
        }

        str& operator *=(usize n);

        str operator *(usize n) const;


        explicit operator std::string() const;

        //inserts s at pos
        void insert(const str& s, usize pos);


        //inserts x at pos
        template<typename T, std::enable_if_t<!std::is_same_v<T, str>, int> = 0>
        void insert(T x, usize pos) {
            str s;
            s.append(x);
            insert(s, pos);
        }

        //erases chars from start to start + min(n, len())
        void erase(usize start, usize n = npos);

        //replaces the chars from start to start + min(len, len()) with s
        void replace(str& s, usize start, usize len = 1);

        //replaces start to start + min(len, len()) with x
        template<typename T, std::enable_if_t<!std::is_same_v<T, str>, int> = 0>
        void replace(T x, usize start, usize len = 1) {
            str to_replace;
            to_replace.append(x);
            replace(to_replace, start, len);
        }

        //replaces up to max copies of _str with _new
        str& replace_all(const str &_str, const str &_new, usize max = npos);

        void swap(str &s) noexcept;

        //removes the last char
        str& pop_back();

        //returns a std::string representing the current string
        std::string stdStr();

        //returns the internal c-string buffer
        const char* data() const;

        //returns a allocated copy of the string as a char*, must use delete[]
        char* c_str() const;


        //some utilities

        //copies chars from begin up to begin + min(n, len()) into buf
        void copy(char* buf, usize begin, usize n = npos) const;

        //finds the first occurrence of s inside the range
        NODISCARD usize find(str s, usize begin = 0, usize end = npos) const;

        //finds the last occurrence of s inside the range
        NODISCARD usize rfind(str s, usize begin = 0, usize end = npos) const;

        //returns a substr of this string
        NODISCARD str substr(usize start, usize n = npos) const;

        //returns a version of this string capitalized by replacing the first alphabetical char with an uppercase version
        NODISCARD str capitalized() const;

        //capializes the string in the same way as capitalized
        str& capitalize();

        //returns a version of the string where all chars have been made uppercase
        NODISCARD str uppercase() const;

        //makes this string all uppercase
        str& toUppercase();

        //returns a version of this string thats all lowercase
        NODISCARD str lowercase() const;

        //makes this string all lowercase
        str& toLowercase();

        //counts the number of occurrences of s in the string
        NODISCARD usize count(const str& s) const;

        //returns if the string ends with the specified suffix
        NODISCARD bool endswith(const str& suffix) const;

        //returns if the string starts with the specified prefix
        NODISCARD bool startswith(const str& prefix) const;


        /*
         * Format a string using c-style formating
         * ex:
         * str s = "3+4 = %d, the area of a circle, radius 3 is %.3f";
         * str s2 = s.format(3+4, 2*M_PI*3*3);
         * cout << s2 << "\n";
         *
         * Result:
         * 3+4 = 7, the area of a circle, radius 3 is 56.548
         */
        NODISCARD str format(...) const;

        //splits the string at each delimiter char
        NODISCARD std::vector<str> split(const str &delimiter = " ", usize max = npos) const;

        //removes all whitespace from the string (spaces, tabs etc)
        str& removeWhitespace();

        //fills the beginning of the string with count copies of c
        str& fill(char c, usize count);

        //fills the back of the string with count copies of c
        str& rfill(char c, usize count);


        NODISCARD i64 compare(const str &s) const;
        NODISCARD i64 compare(const std::string& s) const;
        NODISCARD i64 compare(const char* s) const;
        NODISCARD i64 compare(const str& s, usize start, usize n = npos) const;

        bool operator ==(str other) const;
        bool operator !=(str other) const;
        bool operator <(str other) const;
        bool operator <=(str other) const;
        bool operator >(str other) const;
        bool operator >=(str other) const;

        static void swap(str& s1, str& s2) noexcept;

        [[nodiscard]] iterator begin() const {
            return iterator(cstr);
        }

        [[nodiscard]] iterator end() const {
            return iterator(&cstr[slength]);
        }

        [[nodiscard]] reverse_iterator rbegin() const {
            return reverse_iterator(&cstr[slength-1]);
        }

        [[nodiscard]] reverse_iterator rend() const {
            return reverse_iterator(cstr-1);
        }
    };

    class wstr;

    template<typename T>
    concept WStringifieable = requires(T t)
    {
        {t.toWStr()} -> std::same_as<wstr>;
    };


    NODISCARD extern AUSTINUTILS str operator ""_str(const char* cs, usize);


    NODISCARD extern AUSTINUTILS str operator +(const char* cs, const str& s);

    template<Arithmetic T>
    str v2<T>::toStr() {
        return "<"_str + x + ", " + y + ">";
    }

    template<Arithmetic T>
    str v3<T>::toStr() {
        return "<"_str + x + ", " + y + ", " + z + ">";
    }


    extern AUSTINUTILS int stoi(str& s);
    extern AUSTINUTILS long stol(str& s);
    extern AUSTINUTILS long long stoll(str& s);
    extern AUSTINUTILS unsigned long stou(str& s);
    extern AUSTINUTILS unsigned long stoul(str& s);
    extern AUSTINUTILS unsigned long long stoull(str& s);
    extern AUSTINUTILS float stof(str& s);
    extern AUSTINUTILS double stod(str& s);
    extern AUSTINUTILS long double stold(str& s);



}



template<>
struct std::hash<AustinUtils::str> {
    AustinUtils::usize operator()(const AustinUtils::str& s) const noexcept {
        AustinUtils::usize hash = 5381;
        for (const char& c: s) {
            hash = ((hash << 5) + hash) + c;
        }

        return hash;
    }
};


namespace std {
    extern AUSTINUTILS std::istream& getline(std::istream& is, AustinUtils::str& s, char delim = '\n');
}

#endif