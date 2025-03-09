#include "str.hpp"
#include "Error.hpp"


namespace AustinUtils {
    void str::dealloc() {
        if (cstr != null) {
            delete[] cstr;
            slength = 0;
            msize = 0;
            cstr = null;
        }
    }

    void str::simple_dealloc() {
        delete[] cstr;
        cstr = null;
    }

    str::~str() {
        dealloc();
    }

    str::str() {
        cstr = new char[1];
        cstr[0] = '\0';
        slength = 0;
        msize = 1;
    }

    str::str(const char *c_str) {
        if (c_str == null) throw Exception("Cannot create AustinUtils::str from null character array");
        slength = strlen(c_str);
        msize = slength+5;
        cstr = new char[msize];
        std::memcpy(cstr, c_str, slength);
        cstr[slength] = '\0';
    }

    str::str(const char c, const usize count) {
        slength = count;
        msize = slength+5;
        cstr = new char[msize];
        for (usize i = 0; i < slength; i++) {
            cstr[i] = c;
        }
        cstr[slength] = '\0';
    }

    str::str(const std::string& s) {
        slength = s.length();
        msize = s.length()+5;
        const char* c_str = s.c_str();
        cstr = new char[msize];
        std::memcpy(cstr, c_str, slength);
        cstr[slength] = '\0';
    }

    str::str(const str &s) {
        slength = s.slength;
        msize = s.msize;
        cstr = new char[msize];
        std::memcpy(cstr, s.cstr, slength);
        cstr[slength] = '\0';
    }

    str::str(str &&s) noexcept {
        slength = s.slength;
        msize = s.msize;
        cstr = s.cstr;
        s.cstr = null;
        s.slength = 0;
        s.msize = 0;
    }

    str::str(const std::initializer_list<char> il) {
        slength = il.size();
        auto it = il.begin();
        msize = slength+5;
        cstr = new char[msize];
        for (usize i = 0; i < slength && it != il.end(); i++) {
            cstr[i] = *it;
            ++it;
        }
        cstr[slength] = '\0';
    }

    str::str(const char *c_str, usize n) {
        if (c_str == null) throw Exception("Cannot create AustinUtils::str from null character array");
        if (n > strlen(c_str)) n = strlen(c_str);
        slength = n;
        msize = slength+5;
        cstr = new char[msize];
        std::memcpy(cstr, c_str, slength);
        cstr[slength] = '\0';
    }

    str::str(const str &s, usize start, usize len) {
        if (!s.data()) throw Exception("Invalid AustinUtils::str");
    
        if (start >= s.len()) {
            throw Exception("Cannot access element at index ", start);
        }
        // Calculate the correct substring length
        slength = std::min(len, s.len() - start);
        msize = slength + 5;
        cstr = new char[msize];

        // Copy the substring safely
        std::memcpy(cstr, s.data() + start, slength);
        cstr[slength] = '\0'; // Null-terminate the new string
    }


    str &str::assign(const str &s) {
        return this->operator=(s);
    }

    str &str::assign(const str &s, const usize start, const usize len) {
        return this->operator=(str(s, start, len));
    }

    str &str::assign(const char *c_str) {
        return this->operator=(str(c_str));
    }

    str &str::assign(const char *c_str, usize n) {
        return this->operator=(str(c_str, n));
    }

    str &str::assign(std::initializer_list<char> il) {
        return this->operator=(str(il));
    }

    str &str::assign(char c, usize n) {
        return this->operator=(str(c, n));
    }

    str &str::assign(const str &&s) {
        return this->operator=(s);;
    }


    str& str::operator=(const str &other) {
        if (&other != this) {
            dealloc();
            slength = other.slength;
            msize = other.msize;
            cstr = new char[msize];
            std::memcpy(cstr, other.cstr, slength);
            cstr[slength] = '\0';
        }
        return *this;
    }

    str& str::operator=(str&& other) noexcept {
        if (&other != this) {
            dealloc();
            slength = other.slength;
            msize = other.msize;
            cstr = other.cstr;
            other.cstr = null;
            other.slength = 0;
            other.msize = 0;
        }
        return *this;
    }

    str &str::operator*=(const usize x) {
        if (usize new_length = this->slength * x; new_length >= this->msize) {
            resize(std::max(msize*2, new_length+5));
        }
        const char* temp = c_str();
        for (usize i = 0; i < x-1; i++) {
            this->append(temp);
        }
        delete[] temp;
        return *this;
    }

    str str::operator*(const usize n) const {
        str copy = *this;
        copy*=n;

        return copy;
    }


    usize str::len() const {
        return slength;
    }

    usize str::capacity() const {
        return msize;
    }

    str str::format(...) const {
        va_list vl;
        va_start(vl, this);
        usize needed = vsnprintf(nullptr, 0, cstr, vl);
        va_end(vl);

        if (needed < 0) {
            throw Exception("Error formatting string \"", *this, "\"");
        }

        va_start(vl, this);
        auto buffer = new char[needed+1];
        vsnprintf(buffer, needed+1, cstr, vl);
        va_end(vl);

        auto ret = str(buffer);
        delete[] buffer;

        return ret;
    }

    str &str::capitalize() {
        bool capitalized = false;
        for (usize i = 0; i < slength; i++) {
            if (std::isalpha(cstr[i]) && !capitalized) {
                if (cstr[i] >= 'a') cstr[i] = _toupper(cstr[i]);
                capitalized = true;
            } else if (std::isalpha(cstr[i])) {
                if (cstr[i] <= 'Z') cstr[i] = _tolower(cstr[i]);
            }
        }

        return *this;
    }



    void str::resize(usize n) {
        if (n == msize) return;
        if (n == 0) {
            n = 1;
        }
        msize = n;
        usize plen = slength;
        if (slength > n) slength = n;

        char* c_str = new char[n];

        //copy the data
        std::memcpy(c_str, cstr, std::min(n, plen));
        c_str[slength] = '\0';

        //deallocate old memory
        simple_dealloc();
        //set cstr to the new allocation
        cstr = c_str;
    }

    void str::clear() {
        cstr[0] = '\0';
        slength = 0;
    }

    bool str::empty() const {
        if (cstr != null) return cstr[0] == '\0';
        throw Exception("Cannot check if invalid string is empty");
    }

    void str::shrink_to_fit() {
        if (slength == 0) resize(1);
        if (msize == slength) return;
        resize(slength+1);
    }

    void str::append(const str &s) {
        //check if we have enough room to append
        if (slength + s.slength >= msize) {
            //resize if so, using exponential resizing
            usize new_size = std::max(msize * 2, slength + s.slength + 1);  // Ensure room for null terminator
            resize(new_size);
        }

        std::memcpy(&cstr[slength], s.cstr, s.slength);

        slength += s.slength;
        cstr[slength] = '\0';
    }

    void str::append(const char *s) {
        //check if we have enough room to append
        usize len = strlen(s);
        if (slength + len >= msize) {
            //resize if so, using exponential resizing
            usize new_size = std::max(msize * 2, slength + len + 1);  // Ensure room for null terminator
            resize(new_size);
        }

        std::memcpy(&cstr[slength], s, len);

        slength += len;
        cstr[slength] = '\0';
    }

    void str::append(const std::string& s) {
        //check if we have enough room to append
        usize len = s.length();

        if (slength + len >= msize) {
            //resize if so, using exponential resizing
            usize new_size = std::max(msize * 2, slength + len + 1);  // Ensure room for null terminator
            resize(new_size);
        }

        std::memcpy(&cstr[slength], s.c_str(), len);

        slength += len;
        cstr[slength] = '\0';
    }

    void str::append(char c) {
        if (slength+1 >= msize) {
            resize(msize*2);
        }

        cstr[slength] = c;

        cstr[++slength] = '\0';
    }

    str::operator std::string() const {
        return {cstr};
    }


    void str::insert(const str &s, usize pos) {
        if (s.slength == 0) return;
        // Ensure position is within bounds
        if (pos > slength) {
            throw Exception("Insert position is out of bounds. pos= " + std::to_string(pos));
        }
        if (pos == slength) {
            append(s);
            return;
        }

        if (slength + s.slength > msize) {
            resize(std::max(msize*2, slength+s.slength+1));
        }

        for (usize i = slength+1; i > pos; i--) {
            cstr[i-1+s.slength] = cstr[i-1];
        }
        std::memcpy(&cstr[pos], s.cstr, s.slength);
        slength += s.slength;
        cstr[slength] = '\0';


    }

    void str::erase(usize start, usize n) {
        if (start > slength) throw Exception("Cannot access elements at " + std::to_string(start));
        if (n > slength - start || n == npos) {
            cstr[start] = '\0';
            slength = start;
            return;
        }
        for (usize i = start+n; i < slength; i++) {
            cstr[i-n] = cstr[i];
        }
        slength -= n;
        cstr[slength] = '\0';
    }

    void str::replace(str &s, usize start, usize len) {
        //make sure s is atleast length len
        erase(start, len);
        insert(s, start);
    }

    void str::swap(str &s) noexcept {
        str temp = std::move(*this);
        *this = std::move(s);
        s = std::move(temp);
    }

    str &str::pop_back() {
        if (slength == 0) return *this;
        cstr[slength-1] = '\0';
        slength--;

        return *this;
    }

    std::string str::stdStr() {
        return {cstr};
    }

    char* str::c_str() const {
        char* ret = new char[slength+1];
        std::memcpy(ret, cstr, slength);
        ret[slength] = '\0';

        return ret;
    }

    const char *str::data() const {
        return cstr;
    }

    void str::copy(char *buf, const usize begin, const usize n) const {
        if (begin >= slength) throw Exception("Cannot access elements at " + std::to_string(begin));

        std::memcpy(buf, &cstr[begin], std::min(slength-begin, n));
    }

    usize str::find(const str s, const usize begin, usize end) const {
        end = std::min(slength, end);
        if (begin >= end) return npos;
        if (s.slength > end-begin) return npos;
        for (usize i = begin; i <= end - s.slength; i++) {
            for (usize j = i; j < i+s.slength; j++) {
                if (at(j) != s[j-i]) {
                    goto end_loop;
                }
            }
            return i;

            end_loop:
            continue;
        }
        return npos;
    }

    usize str::rfind(const str s, const usize begin, usize end) const {
        // Ensure `end` is within bounds
        if (end == npos || end > slength) end = slength;

        // Ensure `begin` is valid
        if (begin >= end) return npos;

        // Ensure `s` can fit within the search range
        if (s.slength > (end - begin)) return npos;

        // Search from `end - s.slength` down to `begin`
        for (usize i = end - s.slength; i >= begin; i--) {
            for (usize j = 0; j < s.slength; j++) {
                if (cstr[i + j] != s.cstr[j]) {
                    goto end_loop;
                }
            }
            return i; // Match found

            end_loop:
                if (i == 0) break; // Prevent underflow since `usize` is unsigned
        }

        return npos; // No match found
    }



    i64 str::compare(const str& s) const {

        for (usize i = 0; i < s.slength && i < slength; i++) {
            if (s[i] != at(i)) {
                //return the difference between the 2 chars
                return cstr[i] - s.cstr[i];
            }
        }

        if (s.slength != slength) return cast(s.slength, i64)-cast(slength, i64);

        return 0;
    }

    i64 str::compare(const char *s) const {
        const usize len = strlen(s);
        for (usize i = 0; i < len && i < slength; i++) {
            if (s[i] != at(i)) {
                //return the difference between the 2 chars
                return cstr[i] - s[i];
            }
        }

        if (len != slength) return cast(len, i64)-cast(slength, i64);

        return 0;
    }

    i64 str::compare(const std::string& s) const {
        const usize len = s.length();
        for (usize i = 0; i < len && i < slength; i++) {
            if (s[i] != cstr[i]) {
                //return the difference between the 2 chars
                return cstr[i] - s[i];
            }
        }

        if (len != slength) return cast(len, i64)-cast(slength, i64);

        return 0;
    }

    i64 str::compare(const str &s, usize start, usize n) const {

        if (start >= slength) {
            return -1;
        }

        //so start+n doesnt exceed npos
        if (n > npos-start) n = npos-start;

        for (usize i = start; i < min<usize>(start+n, slength, s.slength+start); i++) {
            if (cstr[i] != s[i-start]) {
                return cstr[i]-s[i-start];
            }
        }

        if (s.slength != min<usize>(n, slength-start)) return cast(s.len(), i64)-cast(slength, i64);

        return 0;
    }


    str str::substr(const usize start, usize n) const {
        if (start > slength) throw Exception("Cannot access elements at " + std::to_string(start));

        str ret;
        //+1 for null
        ret.resize(n+1);

        if (n > npos-start) n = npos-start;

        for (usize i = start; i < start+n && i < slength; i++) {
            ret.append(cstr[i]);
        }

        return ret;
    }

    bool str::operator==(const str other) const {
        return compare(other) == 0;
    }

    bool str::operator!=(const str other) const {
        return compare(other) != 0;
    }

    bool str::operator<(const str other) const {
        return compare(other) < 0;
    }

    bool str::operator>(const str other) const {
        return compare(other) > 0;
    }

    bool str::operator<=(const str other) const {
        return compare(other) <= 0;
    }

    bool str::operator>=(const str other) const {
        return compare(other) >= 0;
    }

    void str::swap(str &s1, str &s2) noexcept {
        s1.swap(s2);
    }

    str str::capitalized() const {
        str ret = *this;
        ret.capitalize();
        return ret;
    }

    str &str::fill(const char c, const usize count) {
        //allocate enough space for the new chars
        if (slength+count >= msize) {
            resize(std::max(slength+count+5, msize*2));
        }

        //move all the chars back by count
        memmove(cstr+count, cstr, slength+1*sizeof(char));
        std::fill_n(cstr, count, c);

        slength+=count;

        return *this;
    }

    str str::lowercase() const {
        str ret = *this;
        ret.toLowercase();
        return ret;
    }

    str &str::rfill(const char c, const usize count) {
        //allocate enough space for the new chars
        if (slength+count >= msize) {
            resize(std::max(slength+count+5, msize*2));
        }

        for (usize i = slength; i < slength+count; i++) {
            cstr[i] = c;
        }
        slength += count;
        cstr[slength] = '\0';

        return *this;
    }

    std::vector<str> str::split(const str &delimiter, const usize max) const {
        std::vector<str> tokens;
        usize pos = 0;
        usize start = 0;
        while ((pos = find(delimiter, start)) != npos && tokens.size() < max-1) {
            str token = substr(start, pos-start);
            tokens.push_back(token);
            start = pos+delimiter.len();
        }
        if (start < slength) tokens.push_back(substr(start));

        return tokens;
    }

    str str::uppercase() const {
        str ret = *this;
        ret.toUppercase();
        return ret;
    }

    str &str::removeWhitespace() {
        usize c = 0;
        for (usize i = 0; i <= slength; i++) {
            if (!iswspace(cstr[i])) {
                cstr[i - c] = cstr[i];
            } else {
                c++;
            }
        }
        slength -= c;
        cstr[slength] = '\0';
        return *this;
    }

    str &str::toLowercase() {
        for (usize i = 0; i < slength; i++) {
            if (std::isalpha(cstr[i])) {
                cstr[i] = cstr[i] <= 'Z' ? _tolower(cstr[i]):cstr[i];
            }
        }

        return *this;
    }

    str &str::toUppercase() {
        for (usize i = 0; i < slength; i++) {
            if (std::isalpha(cstr[i])) {
                cstr[i] = cstr[i] >= 'a' ? _toupper(cstr[i]):cstr[i];
            }
        }

        return *this;
    }

    usize str::count(const str &s) const {
        usize c = 0;
        if (slength < s.slength) {
            return 0;
        }
        usize i;
        usize start = 0;
        while ((i = find(s, start)) != npos) {
            c++;
            start += s.slength;
        }

        return c;
    }

    bool str::endswith(const str &suffix) const {
        if (slength < suffix.slength) return false;

        return compare(suffix, slength-suffix.slength) == 0;
    }

    bool str::startswith(const str &prefix) const {
        if (slength < prefix.slength) return false;

        return compare(prefix, 0, prefix.slength) == 0;
    }

    str& str::replace_all(const str &_str, const str &_new, const usize max) {
        if (max == 0) return *this;
        if (_str.empty()) return *this;
        usize c = 0;

        for (usize i = 0; i <= slength-_str.slength && c < max; i++) {
            if (compare(_str, i, _str.len()) == 0) {
                c++;
                erase(i, _str.slength);
                if (_new.slength > 0) {
                    insert(_new, i);
                    i+=_new.slength-1;//to prevent recursion if _new contains _str
                }
            }
        }


        return *this;
    }




    AUSTINUTILS int stoi(str& s) {
        int result = 0;
        int sign = 1;
        const char* cstr = s.data(); // Use .data() to avoid allocation

        // Handle optional leading whitespace
        while (*cstr == ' ') ++cstr;

        // Handle sign
        if (*cstr == '-' || *cstr == '+') {
            sign = (*cstr == '-') ? -1 : 1;
            ++cstr;
        }

        // Convert characters to integer with overflow handling
        while (*cstr >= '0' && *cstr <= '9') {
            int digit = *cstr - '0';

            // Check for overflow before updating result
            if (result > (INT_MAX - digit) / 10) {
                return (sign == 1) ? INT_MAX : INT_MIN;
            }

            result = result * 10 + digit;
            ++cstr;
        }

        return result * sign;
    }

    AUSTINUTILS long stol(str& s) {
        long result = 0;
        int sign = 1;
        const char* cstr = s.data(); // Use .data() to avoid allocation

        // Handle optional leading whitespace
        while (*cstr == ' ') ++cstr;

        // Handle sign
        if (*cstr == '-' || *cstr == '+') {
            sign = (*cstr == '-') ? -1 : 1;
            ++cstr;
        }

        // Convert characters to integer with overflow handling
        while (*cstr >= '0' && *cstr <= '9') {
            int digit = *cstr - '0';

            // Check for overflow before updating result
            if (result > (T_MAX(long) - digit) / 10) {
                return (sign == 1) ? T_MAX(long) : T_MIN(long);
            }

            result = result * 10 + digit;
            ++cstr;
        }

        return result * sign;
    }

    AUSTINUTILS long long stoll(str& s) {
        long long result = 0;
        int sign = 1;
        const char* cstr = s.data(); // Use .data() to avoid allocation

        // Handle optional leading whitespace
        while (*cstr == ' ') ++cstr;

        // Handle sign
        if (*cstr == '-' || *cstr == '+') {
            sign = (*cstr == '-') ? -1 : 1;
            ++cstr;
        }

        // Convert characters to integer with overflow handling
        while (*cstr >= '0' && *cstr <= '9') {
            int digit = *cstr - '0';

            // Check for overflow before updating result
            if (result > (T_MAX(long long) - digit) / 10) {
                return (sign == 1) ? T_MAX(long long) : T_MIN(long long);
            }

            result = result * 10 + digit;
            ++cstr;
        }

        return result * sign;
    }

    AUSTINUTILS unsigned long stou(str& s) {
        unsigned int result = 0;
        const char* cstr = s.data(); // Use .data() to avoid allocation

        // Handle optional leading whitespace
        while (*cstr == ' ') ++cstr;

        // Handle sign
        if (*cstr == '-' || *cstr == '+') {
            ++cstr;
        }

        // Convert characters to integer with overflow handling
        while (*cstr >= '0' && *cstr <= '9') {
            int digit = *cstr - '0';

            // Check for overflow before updating result
            if (result > (T_MAX(unsigned int) - digit) / 10) {
                return T_MAX(unsigned int);
            }

            result = result * 10 + digit;
            ++cstr;
        }

        return result;
    }

    AUSTINUTILS unsigned long stoul(str& s) {
        unsigned long result = 0;
        const char* cstr = s.data(); // Use .data() to avoid allocation

        // Handle optional leading whitespace
        while (*cstr == ' ') ++cstr;

        // Handle sign
        if (*cstr == '-' || *cstr == '+') {
            ++cstr;
        }

        // Convert characters to integer with overflow handling
        while (*cstr >= '0' && *cstr <= '9') {
            int digit = *cstr - '0';

            // Check for overflow before updating result
            if (result > (T_MAX(unsigned long) - digit) / 10) {
                return T_MAX(unsigned long);
            }

            result = result * 10 + digit;
            ++cstr;
        }

        return result;
    }

    AUSTINUTILS unsigned long long stoull(str& s) {
        unsigned long long result = 0;
        const char* cstr = s.data(); // Use .data() to avoid allocation

        // Handle optional leading whitespace
        while (*cstr == ' ') ++cstr;

        // Handle sign
        if (*cstr == '-' || *cstr == '+') {
            ++cstr;
        }

        // Convert characters to integer with overflow handling
        while (*cstr >= '0' && *cstr <= '9') {
            int digit = *cstr - '0';

            // Check for overflow before updating result
            if (result > (T_MAX(unsigned long long) - digit) / 10) {
                return T_MAX(unsigned long long);
            }

            result = result * 10 + digit;
            ++cstr;
        }

        return result;
    }

    AUSTINUTILS float stof(str& s) {
        float result = 0.0f;
        int sign = 1;
        bool hasDecimal = false;
        float decimalMultiplier = 0.1f;
        const char* cstr = s.data(); // Use .data() to avoid allocation

        // Handle optional leading whitespace
        while (*cstr == ' ') ++cstr;

        // Handle sign
        if (*cstr == '-' || *cstr == '+') {
            sign = (*cstr == '-') ? -1 : 1;
            ++cstr;
        }

        // Convert characters to float with decimal support
        while ((*cstr >= '0' && *cstr <= '9') || *cstr == '.') {
            if (*cstr == '.') {
                if (hasDecimal) break; // Only one decimal point allowed
                hasDecimal = true;
            } else {
                int digit = *cstr - '0';

                if (!hasDecimal) {
                    result = result * 10.0f + digit;
                } else {
                    result += digit * decimalMultiplier;
                    decimalMultiplier *= 0.1f;
                }
            }
            ++cstr;
        }

        result *= sign;



        return result;
    }

    AUSTINUTILS double stod(str& s) {
        double result = 0.0f;
        int sign = 1;
        bool hasDecimal = false;
        double decimalMultiplier = 0.1f;
        const char* cstr = s.data(); // Use .data() to avoid allocation

        // Handle optional leading whitespace
        while (*cstr == ' ') ++cstr;

        // Handle sign
        if (*cstr == '-' || *cstr == '+') {
            sign = (*cstr == '-') ? -1 : 1;
            ++cstr;
        }

        // Convert characters to float with overflow handling
        while ((*cstr >= '0' && *cstr <= '9') || (*cstr == '.' && !hasDecimal)) {
            if (*cstr == '.') {
                hasDecimal = true;
            } else {
                int digit = *cstr - '0';
                if (!hasDecimal) {
                    // Check for overflow before updating result
                    if (result > (T_MAX(double) - digit) / 10.0f) {
                        return (sign == 1) ? T_MAX(double) : T_MIN(double);
                    }
                    result = result * 10.0f + digit;
                } else {
                    result += digit * decimalMultiplier;
                    decimalMultiplier *= 0.1f;
                }
            }
            ++cstr;
        }

        result *= sign;



        return result;
    }

    AUSTINUTILS long double stold(str& s) {
        long double result = 0.0f;
        int sign = 1;
        bool hasDecimal = false;
        long double decimalMultiplier = 0.1f;
        const char* cstr = s.data(); // Use .data() to avoid allocation

        // Handle optional leading whitespace
        while (*cstr == ' ') ++cstr;

        // Handle sign
        if (*cstr == '-' || *cstr == '+') {
            sign = (*cstr == '-') ? -1 : 1;
            ++cstr;
        }

        // Convert characters to float with overflow handling
        while ((*cstr >= '0' && *cstr <= '9') || (*cstr == '.' && !hasDecimal)) {
            if (*cstr == '.') {
                hasDecimal = true;
            } else {
                int digit = *cstr - '0';
                if (!hasDecimal) {
                    // Check for overflow before updating result
                    if (result > (T_MAX(long double) - digit) / 10.0f) {
                        return (sign == 1) ? T_MAX(long double) : T_MIN(long double);
                    }
                    result = result * 10.0f + digit;
                } else {
                    result += digit * decimalMultiplier;
                    decimalMultiplier *= 0.1f;
                }
            }
            ++cstr;
        }

        result *= sign;



        return result;
    }

    str operator+(const char *cs, const str &s) {
        return str(cs) + s;
    }

    str operator ""_str(const char *cs, usize) {
        return str(cs);
    }



}

namespace std {
    AUSTINUTILS std::istream& getline(std::istream& is, AustinUtils::str& s, char delim) {
        s.clear();
        char c;

        if(is.get(c)){
            s.append(c);
            while((is.get(c)) && (c != delim))
                s.append(c);
            if(!is.bad() && is.eof())
                is.clear(std::ios_base::eofbit);
        }
        return is;
    }


}