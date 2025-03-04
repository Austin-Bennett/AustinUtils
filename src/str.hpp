#ifndef STR_HPP
#define STR_HPP
#include <misc.hpp>
#include "math.hpp"

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

        str();

        str(const char* c_str);

        str(char c, usize count);

        str(const std::string& s);

        str(const str& s);

        str(str&& s) noexcept ;

        str& operator =(const str& other);

        str& operator =(str&& other) noexcept ;

        NODISCARD usize len() const;

        NODISCARD usize max_size() const;

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

        void append(const str& s);

        void append(const char* s);

        void append(const std::string& s);

        void append(char c);

        template<Arithmetic T>
        void append(T x) {
            append(std::to_string(x));
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

        explicit operator std::string() const;

        void insert(const str& s, usize pos);

        template<typename T, std::enable_if_t<!std::is_same_v<T, str>, int> = 0>
        void insert(T x, usize pos) {
            str s;
            s.append(x);
            insert(s, pos);
        }

        void erase(usize start, usize n = npos);

        void replace(str& s, usize start, usize len = 1);

        template<typename T, std::enable_if_t<!std::is_same_v<T, str>, int> = 0>
        void replace(T x, usize start, usize len = 1) {
            str to_replace;
            to_replace.append(x);
            replace(to_replace, start, len);
        }

        void swap(str &s);

        void pop_back();

        //returns a std::string representing the current string
        std::string stdStr();

        //returns the internal c-string buffer
        const char* data() const;

        //returns a allocated copy of the string as a char*, must use delete[]
        char* c_str() const;

        void copy(char* buf, usize begin, usize n = npos) const;

        NODISCARD usize find(str s, usize begin = 0, usize end = npos) const;

        NODISCARD usize rfind(str s, usize begin = 0, usize end = npos) const;

        NODISCARD str substr(usize start, usize n = npos) const;

        NODISCARD i64 compare(const str &s) const;
        NODISCARD i64 compare(const std::string& s) const;
        NODISCARD i64 compare(const char* s) const;

        bool operator ==(str other) const;
        bool operator !=(str other) const;
        bool operator <(str other) const;
        bool operator <=(str other) const;
        bool operator >(str other) const;
        bool operator >=(str other) const;

        static void swap(str& s1, str& s2) noexcept;

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




    inline str operator +(const char* cs, str& s) {
        return str(cs) + s;
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