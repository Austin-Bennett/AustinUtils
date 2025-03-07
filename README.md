# Austin Utils
**A utilities library, with several features**

**Namespace: AustinUtils**

*You can get the current version using* `AustinUtils::AustinUtilsVersion()` *which will return as a* `AustinUtils::str`


# Error

**Contains:**
`class Error : public runtime_error`
`class Exception : public Error`

Both classes use the excellent **boost::stacktrace** library to add a stacktrace to regular exceptions
**Usage:**
```
//regular error, throws a simple string message
throw AustinUtils::Error(const AustinUtils::str& msg)

//throws a formatted error
throw AustinUtils::Exception(Args... format_args)

//Example

throw AustinUtils::Error("Could not find foo");
/*
Output:
terminate called after throwing an instance of 'AustinUtils::Error'
  what():  Could not find foo
*stacktrace*
*/

int index = 4;
throw AustinUtils::Exception("No foo found at index ", index);

/*
Output:
terminate called after throwing an instance of 'AustinUtils::Exception'
  what():  No foo found at index 4
*stacktrace*
*/
```

# Logging

**Contains:**
```
enum LOG_TYPE {
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_DEBUG
}
std::string LogTypeToString(LOG_TYPE typ)
std::wstring LogTypeToWString(LOG_TYPE typ)
class logger
class wlogger
```

`wlogger` **is a version of** `logger` **but uses wide strings, otherwise, both have exactly the same methods**

**string_type means either std::wstring or std::string depending on which logger object you use**

**char_type means either char*** **or** **wchar_t*** **depending on the logger object used**

| Method | Description |
|:---:|:---:|
| `logger(const string_type& name)` | creates a new logger object with the name `name` |
| `void c_log(LOG_TYPE type, const char_type fmt, ...)` | logs a new message using c-style formatting in the form `"[LOG_TYPE][name]: message"` |
| `void c_log(LOG_TYPE type, const char_type fmt, va_list args)` | logs a new message using c-style formatting with a va_list in the form `"[LOG_TYPE][name]: message"` |
| `void log(LOG_TYPE typ, Args... args)` | logs a new message in the form: `"[LOG_TYPE][name]: message"` |
| `void info(Args... args)` | logs a new message in the form `"[INFO][name]: message"` |
| `void warn(Args... args)` | logs a new message in the form `"[WARN][name]: message"` |
| `void error(Args... args)` | logs a new message in the form `"[ERROR][name]: message"` |
| `void debug(Args... args)` | logs a new message in the form `"[DEBUG][name]: message"` |

# Math

**Contains:**
```
//for distinguishing between angle types
typedef double arcdegrees
typedef double radians

//describes any numerical type
concept Arithmetic

//describes any integer type signed or unsigned
concept Integral

//describes any floating-point number
concept FloatingPoint

//describes any unsigned integer
concept Unsigned

//to be used anywhere a function that can be called in the form f(x) -> y is need
typedef std::function<double(double)> m_function

//simliar to mathmatical sigma notation 
double sum(int n, int stop, m_function fn_x);
//similar to mathmatical capital pi notation
double product(int n, int stop, m_function fn_x);
//converts radians to degrees
arcdegrees toDeg(radians theta);
//convertes degrees to radians
radians toRad(arcdegrees theta);

//sums all the numbers in the iterable
double sum(T iterable)

//computes the product of all numbers in the iterable
double product(T iterable)

template<Arithmetic T>
struct v2;

typedef v2<unsigned short> usvec2;
typedef v2<unsigned int> uvec2;
typedef v2<unsigned long> ulvec2;
typedef v2<unsigned long long> ullvec2;

typedef v2<short> svec2;
typedef v2<int> ivec2;
typedef v2<long> lvec2;
typedef v2<long long> llvec2;

typedef v2<float> fvec2;
typedef v2<double> dvec2;
typedef v2<long double> ldvec2;

class complex

//computes the complex number z to the real power n
complex pow(complex z, double n)
//NOT IMPLEMENTED, DO NOT USE
complex pow(complex z, complex w)

template<Arithmetic T>
struct matrix;
```

**The following method lists for classes do not contain operators, but the operators are defined as is generally accepted by the mathematical community**

**struct v2<T>**
| Method | Description |
| :---: | :---: |
| `static v2<T> of[_deg](angle_type theta, double magnitude)` | creates a vector from an angle and a magnitude |
| `static v2<T> from(complex c)` | creates a vector from a complex number |
| `double length()` | returns the length of the vector |
| `double length2()` | returns the length squared of the vector |
| `angle_type direction[_deg]()` | returns the vectors direction using the angle_type |
| `v2<T> reversed` | returns a reversed version of the vector |
| `v2<T> normalized()` | returns a vector of length 1 |
| `friend std::ostream& operator << (std::ostream& os, v2<T> self` | formats the vector to a output stream in the form <x, y> |
| `v2<aT> convert_data()` | converts the vectors data to a new arithmetic type |
| `double dot(v2<aT> other)` | computes the dot product between the `this` and `other` vectors |


**Note: Massive overhaul coming for this class as it lwk sucks**
**class complex**
| Method | Description |
| :---: | :---: |
| `complex()` | creates the complex number 0+0i  |
| `complex(double real)` | creates the complex number real+0i |
| `complex(double real, double imaginary)` | creates the complex number real + imaginary*i |
| `complex conjugate()` | returns the conjugate of the complex number |
| `bool is_real()` | returns if the complex number is real |
| `double magnitude()` | returns its length |
| `double magnitude2()` | returns its length squared |
| `angle_type direction[_deg]` | returns the direction of the complex number |
| `void convert_if_real(T* buffer)` | if the complex number is real it stores it in the buffer, otherwise it doesnt |
| `friend std::ostream& operator << (std::ostream& os, complex self` | formats the complex number to the output stream in the form a +/- bi |


**struct matrix<T>**
| Method | Description |
| :---: | :---: |
| `virtual ~matrix()` | deallocates the matrix |
| `usize width() const` | returns the number of columns in the matrix |
| `usize height() const` | returnst the number of rows in the matrix |
| `matrix(std::initalizer_list<std::initalizer_list<T>> matrix)` | creates a matrix from the initalizer list matrix |
| `matrix(usize rows, usize columns, T default_value = 0)` | creates a `rows*columns` matrix with all values set to `default value` |
| copy and move constructors and operators | copies/moves the matrix |
| `matrix(iT start, iT end, usize rows, usize columns)` | creates a matrix `rows*columns` and uses the iterator values from start to end for the matrix |
| iteration methods | used for iterating |
| `operator [usize i]` | returns an array at the row index i |

# Str

**
I dont like the standard library string, so I made this one, with support for appending more than just strings, it has (almost) all the functionality from std::string too so almost 0 compromise using it, and even more
features will be added soon, including a wstr type for wide characters
**

**Contains:**
```
//describes any object with a method of signature str toStr()
concept Stringifieable

class str

//defines appending a c-string to a string
str operator + (const char* cs, str& s)

int stoi(str&s)
long stol(str&s)
long long stoll(str&s)
unsigned int stou(str&s)
unsigned long stoul(str&s)
unsigned long long stoull(str&s)
float stof(str&s)
double stod(str&s)
long double stold(str&s)
//hash function for use as a key in std::unordered_map
struct std::hash<AustinUtils::str>
//getline function for the str class
std::istream& getline(std::istream& is, AustinUtils::str& s, char delim = '\n')
```

**class str**

| Methods | Description |
| :---: | :---: |
| `virtual ~str()` | deallocates the string |
| `str()` | creates an empty string |
| `str(const char* c_str)` | creates a string from the c-style string |
| `str(char c, usize count)` | creates a string with `count` number of `c` |
| `str(const std::string& s)` | creates a str from a C++ std::string |
| Move and copy operators and constructors | Copies/Moves a str object |
| `usize len()` | returns the length of the string |
| `usize max_size()` | returns the current maximum capacity of the string |
| `void resize(usize new_size)` | resizes the strings internal array |
| `void clear()` | clears the entire string |
| `bool empty()` | returns if the string is empty or not |
| `void shrink_to_fit()` | resizes the string to be just big enough to contain everything up to its null char |
| `char& at(const usize index) const` | returns the char at the index |
| `char& operator [const usize index]` | returns the char at the index |
| `char& back() const` | returns the char at the back |
| `char& front() const` | returns the char at the front |
| `friend std::ostream& operator<<(std::ostream& os, const str& self)` | outputs the string to the stream |
| `void append(const str& s)` | appends the string to the end of the string |
| `void append(const char* s)` | appends the c-string to the end of the string |
| `void append(std::string s)` | appends the C++ std::string to the end of the string |
| `void append(T x)` | appends the number to the end of the string |
| `void append(T& x)` | appends any Stringifieable object to the end of the string |
| `str& operator +=(T x)` | appends any object that can be appended to the end of the string |
| `str operator+(T x)` | returns a string formed by appending the current string to anything that can be appended |
| `explicit operator std::string() const` | returns the str as a std::string |
| `insert(const str& s, usize pos)` | inserts a str at any valid position |
| `insert(T x, usize pos)` | inserts any valid object or type that can be converted to a string through appendation |
| `void erase(usize start, usize n = npos)` | erases all chars from `start` to `min(n, len())` |
| `void replace(str& s, usize start, usize len)` | replaces a range of characters with `s` |
| `void replace(T x, usize start, usize len)` | replace a range with any object that can be converted to a str |
| `void swap(str&)` | swaps the 2 strings by moving them |
| `void pop_back()` | pops the last character |
| `std::string stdString()` | returns the std::string representation of the str |
| `const char* data()` | returns the internal char buffer array |
| `char* c_str()` | returns a newly allocated c-string representing value of the str |
| `void copy(char* buf, usize begin, usize n = npos)` | copies chars from `begin` to `min(n, len())` |
| `usize find(str s, usize begin = 0, usize end = npos)` | finds the first occurence of s in the string |
| `usize rfind(str s, usize begin = 0, usize end = npos)` | finds the last occurence of s in the string |
| `str substr(usize start, usize n = npos)` | returns a substring from `start` to `min(n, len())` |
| `i64 compare(const string_type s)` | returns a comparison between the 2 strings, 0 if they are equal |
| comparison operators (==, !=, > etc | returns booleans depending on the result of compare between the string and the other string |
| `static void swap(str& s1, str& s2)` | swaps the 2 strings values |
| `friend std::istream& operator >>(std::istream& is, str& s)` | gets a string from an input stream |
| iterator functions | used for iterating through the string |

**stox functions**
Convert a str object into any numerical type

**Hash Function**
allows the str to be used as a key for std::unordered_map and other standard library objects requiring the hash

**getline(std::istream is, AustinUtils::str& s, char delim = '\n')**
Retrieves a line from the input stream and stores in s


# Miscellaneous

**Contains:**
```
//describes anything that can be iterated through
concept Iterable

//anything formattable to a string
concept Formattable

//anything formattable to a wstring
concept WideFormattable

//splits a string into several substrings delimmeted by del
std::vector<std::string split(std::string s, std::string del = " ")>

//formats args into a string
std::string fmt(Args... args)

//formats args into a wstring
std::wstring wfmt(Args... args)

//formats any iterable to an output stream
std::ostream& operator <<(std::ostream& os, const T& vec)

const static UINT og_console_CP = GetConsoleCP();

//sets the console up for printing wstrings or back to regular strings
void SetConsoleUTFMode(const bool value)

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
    #define T_MIN(T) (std::numeric_limits<T>::min())
    
    //i like this rust keyword
    #define loop while(true)

//an RAM iterator that wraps a pointer
class basic_random_access_iterator

//a forward iterator wrapping a pointer
class basic_iterator

//a backwards iterator wrapping a pointer
class basic_reverse_iterator

```



