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

typedef v2<u8> ubvec2;//unsigned byte vector 2
typedef v2<u16> usvec2;//unsigned short vector 2
typedef v2<u32> uvec2;//unsigned vector 2
typedef v2<u64> ullvec2;//unsigned long long vector 2

typedef v2<i8> bvec2;//byte vector 2
typedef v2<i16> svec2;//short vector 2
typedef v2<i32> ivec2;//integer vector 2
typedef v2<i64> llvec2;//long long vector 2

typedef v2<float> fvec2;//float vector 2
typedef v2<double> dvec2;//double vector 2
typedef v2<long double> ldvec2;//long double vector 2

struct v3 : public v2;

typedef v3<i8> bvec3;
typedef v3<i16> svec3;
typedef v3<i32> ivec3;
typedef v3<i64> llvec3;

typedef v3<u8> ubvec3;
typedef v3<u16> usvec3;
typedef v3<u32> uvec3;
typedef v3<u64> ullvec3;

typedef v3<float> fvec3;
typedef v3<double> dvec3;
typedef v3<long double> ldvec3;

class complex

//computes the complex number z to the real power n
complex pow(complex z, double n)
//NOT IMPLEMENTED, DO NOT USE
complex pow(complex z, complex w)

template<Arithmetic T>
struct matrix;
```

**The following method lists for classes do not contain operators, but the operators are defined functionally the same as is the general functional definition accepted by the mathematical community**

**struct v2<T>**
| Method | Description |
| :---: | :---: |
| `static v2<T> of[_deg](angle_type theta, double magnitude)` | creates a vector from an angle and a magnitude |
| `static v2<T> from(complex c)` | creates a vector from a complex number |
| `double length()` | returns the length of the vector |
| `double length2()` | returns the length squared of the vector |
| `angle_type direction[_deg]()` | returns the vectors direction using the angle_type |
| `v2<T> reversed` | returns a reversed version of the vector |
| `v2<T> reverse` | reverses the vecotr |
| `v2<T> normalized()` | returns a vector of length 1 |
| `v2<T> normalize` | normalizes the vector |
| `friend std::ostream& operator << (std::ostream& os, v2<T> self` | formats the vector to a output stream in the form <x, y> |
| `v2<aT> convert_data()` | converts the vectors data to a new arithmetic type |
| `double dot(v2<aT> other)` | computes the dot product between the `this` and `other` vectors |
| `v2 rotated[_deg](angle_type theta, v2 center = {0, 0})` | returns a version of the vector rotated, around the point `center`, by theta |
| `v2& rotate[_deg](angle theta, v2 center = {0, 0})` | rotates the vector, around the point `center`, by theta |


**struct v3<T>**
| Method | Description |
| :---: | :---: |
| `static v2<T> of[_deg](T magnitude, angle xrot, angle yrot, angle zrot)` | creates a vector from a magnitude, and its rotation in 3D space |
| `static v2<T> of[_deg](T magnitude, v3<angle>)` | creates a vector from a magnitude and its rotation in 3D space |
| `T length()` | returns the length of the vector |
| `T length2()` | returns the length squared of the vector |
| `v3<angle> direction[_deg]()` | returns the vectors direction using the angle_type |
| `v3<T> reversed` | returns a reversed version of the vector |
| `v3<T> normalized()` | returns a vector of length 1 |
| `v3<T> reverse` | reverses the vector |
| `v3<T> normalize()` | normalizes the vector |
| `friend std::ostream& operator << (std::ostream& os, v2<T> self` | formats the vector to a output stream in the form <x, y> |
| `v3<aT> convert_data()` | converts the vectors data to a new arithmetic type |
| `T dot(v3<aT> other)` | computes the dot product between the `this` and the `other` vector |
| `v3 crossed(const v3& other)` | returns the cross product between `this` and the `other` vector |
| `v3& crossed(const v3& other)` | returns the cross product between `this` and the `other` vector |
| `v3 rotated[_deg](angle xrot, angle yrot, angle zrot, v3 center = {0, 0, 0})` | returns a new vector rotated around `center` using the rotation values |
| `v3 rotated[_deg](v3<angle> rotator, v3 center = {0, 0, 0})` | returns a new vector rotated around `center` using the rotation values |
| `v3& rotate[_deg](angle xrot, angle yrot, angle zrot, v3 center = {0, 0, 0)` | rotates the vector around `center` |
| `v3& rotate[_deg](v3 rotator, v3 center = {0, 0, 0)` | rotates the vector around `center` |


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

**I dont like the standard library string, so I made this one, with support for appending more than just strings, and lots more modifier methods, 
it has (almost) all the functionality from std::string too meaning there is almost 0 compromise to using it, and even more
features will be added soon, including a wstr type for wide characters**

**Contains:**
```
//describes any object with a method of signature str toStr()
concept Stringifieable

class str

//operator for converting a c-string into a str
str operator ""_str(const char* cs, usize);
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
| `str(const str& s, usize start, usize len = npos)` | creates a string from a substring |
| `str(const char* c_str, usize n)` | creates a string from the first `n` chars of `c_str` |
| `str(iterator begin, iterator end)` | creates a string from an iterator |
| `str(initalizer_list<char> il)` | creates a string from a initalizer array |
| `explicit str(T x)` | creates a string by converting x into a string |
| `assign(...)` | assigns the strings value, uses the same arguments as any constructor except the default constructor |
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
| comparison operators (`==`, `!=`, `>` etc) | returns booleans depending on the result of compare between the string and the other string |
| `static void swap(str& s1, str& s2)` | swaps the 2 strings values |
| `friend std::istream& operator >>(std::istream& is, str& s)` | gets a string from an input stream |
| `str capitalized()` | returns a copy of the string where the first alphabetical char is made uppercase and the rest are made lowercase |
| `str& capitalized()` | capitalized the string |
| `str uppercase()` | returns a copy where all alphabetical chars are in uppercase |
| `str& toUppercase()` | makes the string all uppercase |
| `str lowercase()` | returns a version of the string where all alphabetical chars are in lowercase |
| `str& toLowercase()` | makes the string all lowercase |
| `usize count(const str& s)` | counts the number of occurences of `s` in the string |
| `bool endswith(const str& suffix)` | returns true if the string ends with `suffix` |
| `bool startswith(const str& prefix)` | returns true if the string starts with prefix |
| `str format(...)` | uses the string and the arguments in .format() to format the string using c-style formmating |
| `std::vector<str> split(const str& delimiter - " ", usize max = npos)` | splits the string up to `max` substring split at `delimiter` |
| `str& removeWhitespace()` | removes all whitespace from the string |
| `str& fill(char c, usize count)` | fills the beginning of the string with `count` copies of `c` |
| `str& rfill(char c, usize count)` | fills the back of the string with `count` copies of `c` |
| iterator functions | used for iterating through the string |

**stox functions**
Convert a str object into any numerical type

**Hash Function**
allows the str to be used as a key for std::unordered_map and other standard library objects requiring the hash

**getline(std::istream is, AustinUtils::str& s, char delim = '\n')**
Retrieves a line from the input stream and stores in s

# linkedlist

**Contains:**
```
list_node<T>

std::swap(list_node<T>* n1, list_node<T>* n2)
std::swap(list_node<T>& n1, list_node<T>& n2)
```


**struct list_node**

**`list_node` is a doubly-linked linked list node**

| Methods | Description |
| :---: | :---: |
| `void push_forward(T x)` | creates a new node with value x and inserts it between `this` and `next` |
| `void push_backward(T x)` | creates a bew bide with value x and inserts it between `prev` and `next` |
| `list_node<T>* erase_and_advanc()` | erases the `this` node and returns the next pointer |
| `list_node<T>* erase_and_retreat()` | erases the `this` node and returns the prev pointer |
| `void erase_forward()` | erases the `next` node |
| `void erase_backward()` | erases the `prev` node |
| `void deallocate()` | destroys all nodes in the list, you must call this method when finished with the list as the destructor only destroys the `this` node |
| `std::ostream& operator <<(std::ostream& os, list_node<U>& self)` | formats the list to an output stream |
| `list_node<T>* goToEnd()` | returns the last allocated node in the list |
| `list_node<T>* goToBeginning()` | returns the first allocated node in the list |
| `iterator begin()` | returns an iterator point from `this` towards the end of the list |
| `iterator end()` | returns an iterator pointing to nullptr, since that is technically the end of the list |
| `riterator rbegin()` | returns an iterator pointing from `this` towards the beginning of the list |
| `riterator rend()` | returns a reverse iterator to nullptr, as this is technically the very start of the list aswell |
| `void swap(list_node& other)` | swaps the 2 nodes by swapping their `prev` and `next` pointers |
| `void swap(list_node* other)` | swaps the 2 nodes by swapping their `prev` and `next` pointers |

**Example linked list formatting**
```
list_node<int>* ilist = new list_node<int>({1, 2, 3, 4, 5});
ilist = ilist->next->next;
cout << ilist->x << "\n";
cout << ilist << "\n";
```
**Output:**
```
3
1 <- 2 <- 3 -> 4 -> 5
```

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
#In the future

**Coming in a future update:**
- `class wstr` for wide strings
- Support for every function in std::string inside str

**Ideas that may come in a future update**
- Custom allocator support for AustinUtils::list_node

**Ideas that I have, but wont necessarily be added**
- `class container<T>` a container class that can use any data structure for storing `T`, could be an array structure, a linked list structure, or a bucket structure
