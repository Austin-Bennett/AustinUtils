# AustinUtils

**This is a small utilities library with several features**

**You can get the current version with `const std::string AustinUtilsVersion()`**


# Argument Parser

Parsing arguments made easy with an `argument_parser` object.

| Method | Description |
|:---:|:---:|
| `argument_parser::argument_parser(int argc, char** argv)` | Initalizes the argument_parser object |
| `argument argument_parser::add_argument(std::string name, std::string key, argument_type typ, bool required = true)` | Adds a new argument to the parser |
| `void argument_parser::parse_arguments()` | parses the arguments in argv |
| `std::string get_usage()` | returns a string representing the correct usage of a program |
| `argument get_argument(std::string name)` | returns an argument by its name |

**The argument_type enum:**
```
enum argument_type {
    INT,//aka long long
    UINT,//aka unsigned long long
    FLOAT,//aka long double
    STRING,
    BOOL,//these arguments are always non-required, they also take no parameters, their mere existence makes their value true
    FILE_PATH,//like a string, but these arguments will check if the file exists first aswell
};
```
**The argument_type enum:**
```
enum broad_argument_type {
    NUMERICAL,//floats, ints
    UNSIGNED_NUMERICAL,//unsigned ints
    ANY//for anything else (strings, bools, paths)
};
```
**The argument structure:**
```
struct AUSTINUTILS argument {
    std::string key; //the key that the parser will look for (ex; -d, -f, -s --thing)
    std::string name;//the keys name
    bool required; //whether the parser should expect this argument or not
    argument_type type; //the data type of the argument
    ArgValue data; //ArgValue is actually a std::any
};
```
**Finally, `cast_arg<T>(argument arg)` and `cast_arg_or<T>(argument arg, std::type_identity_t<T> or_else)`:**

These will return the underlying value of the argument.
| Argument Type | Correct usage |
|:---:|:---:|
| `INT` | `cast_arg<ARG_INT>(arg), cast_arg_or<ARG_INT>(arg, long long val)` |
| `UINT` | `cast_arg<ARG_UINT>(arg), cast_arg_or<ARG_UINT>(arg, unsigned long long val)` |
| `FLOAT` | `cast_arg<ARG_FLOAT>(arg), cast_arg_or<ARG_FLOAT>(arg, long double val)` |
| `STRING` | `cast_arg<ARG_STRING>(arg), cast_arg_or<ARG_STRING>(arg, std::string val)` |
| `BOOL` | `cast_arg<ARG_BOOL>(arg), cast_arg_or<ARG_BOOL>(arg, bool val)` |
| `FILE_PATH` | `cast_arg<ARG_PATH>(arg), cast_arg_or<ARG_PATH>(arg, std::string val)` |

It should be noted that using cast_arg_or for a boolean is pretty much useless, as cast_arg will return false if the argument wasnt passed

The ARG_**type** types are used to make casting easy, the code definition is below:
```
using ARG_INT = long long;
using ARG_UINT = unsigned long long;
using ARG_FLOAT = long double;
using ARG_STRING = std::string;
using ARG_BOOL = bool;
using ARG_PATH = std::string;
```

# Example Usage:

```
int main(int argc, char** argv) {
    argument_parser parser(argc, argv);
    
    parser.add_argument("num", "-n", INT);
    parser.add_argument("size", "-s", UINT, false);
    parser.add_argument("pi", "-p", FLOAT);
    parser.add_argument("debug", "-d", BOOL);
    parser.add_argument("file", "-f", FILE_PATH, false);

    //the first string not belonging to any other argument will be mapped to this one
    parser.add_argument("name", "", STRING);

    parser.parse_arguments();

    argument n = parser.get_argument("num");
    argument s = parser.get_argument("size");
    argument p = parser.get_argument("pi");
    argument Name = parser.get_argument("name");
    argument d = parser.get_argument("debug");
    argument f = parser.get_argument("file");
    
    cout << cast_arg_or<ARG_INT>(n, 0) << "\n";
    cout << cast_arg_or<ARG_UINT>(s, 0) << "\n";
    cout << cast_arg_or<ARG_FLOAT>(p, 0.0) << "\n";
    cout << cast_arg_or<ARG_STRING>(N, "") << "\n";
    cout << cast_arg<ARG_BOOL>(d) << "\n";
    cout << cast_arg_or<ARG_PATH>(f, "log.txt") << "\n";
    
}
```

# Logging
Contains a `logger` and a `wlogger` for standard and wide character printing respectivley

Methods:
| Method | Description |
|:---:|:---:|
| `logger(const string& name)` or `wlogger(const wstring& name)` | creates a new logger object with the specified name |
| `c_log(LOG_TYPE type, const charT *fmt, ...)` | logs a message of type `type` using c-style formmating |
| `log(LOG_TYPE typ, Args... args)` | logs a message of type `type` where all elements in `args` are printed sequentially with no delimeters |
| `info, warn, error, debug (Args... args) ` | logs a message where the type depends on the function used, `args` is printed sequentially with no delimeters |

# Math utilities


**Explicit angle types**

```
typedef double arcdegrees;
typedef double radians;
```
These are used in several function definitions to make it clear what kind of angle is expected/returned

**Concepts**

**NOTE: Concepts are a C++20 feature, if you want to use them in your code then, atleast with G++, you need to add the -std=c++20 flag**

```
concept Iterable<T>
concept Arithmetic<T>
```

The Iterable requires objects with the .begin() and .end() methods declared, meaning they are iterable with a for-each loop

The Arithmetic concept requires only numerical types (i.e float, double, int, long long, etc)

**Functions**

`AUSTINUTILS double sum(int n, int stop, m_function fn_x)`

Sums a m_function from n to stop, similar to mathmatical sigma notation

An m_function is just a std::function<double(double)>

`AUSTINUTILS double product(int n, int stop, m_function fn_x)`

Returns the product of all values of y for each value of x in fn_x from n to stop

Similar to mathmatical **capital** pi notation

```
template<Iterable T>
AUSTINUTILS double sum(T iterable)
```

Sums all the values in a iterable container, provided the values are all arithmetic

```
template<Iterable T>
AUSTINUTILS double product(T iterable)
```

Returns the product between 1 and all the numbers in the iterable

**The `v2<Arithmetic T>` structure, or vector2**

It is a 2D vector that stores an x and y, which can be of any arithmetic type

| Method | Description |
| :---: | :---: |
| `static v2<T> of(radians theta, double magnitude)` | returns a vector given its angle in radians and magnitude |
| `static v2<T> of_deg(arcdegrees theta, double magnitude)` | returns a vector given its angle in arcdegrees and magnitude |
| `static v2<double> from(complex c)` | creates a vector from a complex number |
| `double length()` | returns the length of the vector |
| `double length2()` | returns the square length of the vector |
| `radians direction()` | returns the direction of the vector in radians |
| `arcdegrees direction_deg()` | returns the direction of the vector in degrees |
| `v2<T> reversed()` | returns the reversed vector |
| `v2<T> normalized()` | returns the vector normalized, i.e with the same direction but with length 1 |
| `friend std::ostream& operator <<(std::ostream& os, v2<T> self)` | outputs the vector to the std::ostream |
| `friend logger& operator << (logger& lg, v2<T> self)` | outputs the vector to a logger |
| `template<Arithmetic aT> v2<aT> convert_data()` | converts the vector to a vector of type aT |
| `double dot(v2<aT> other)` | returns the dot product between the 2 vectors |
| `operator +,-(v2<aT> other)` | returns the sum, or inverse sum between 2 vectors |
| `operator *,/(v2<aT> other)` | returns the product or inverse product between 2 vectors |
| `operator *,/(aT other)` | returns the product or inverse product between the vector an a number |

**You can also use the operator= variants for all of these**

**You can explicitly define the type of the vector, but these typedefs are also provided**

```
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
```

**The complex class**

A class for complex numbers

To save time writing, all the arithmetic operators are defined and work exactly as how they would in actual mathematics, they also can be formmatted to a ostream or logger

| Method | Description |
|:---:|:---:|
| `complex()` | creates the complex number 0 + 0i |
| `complex(double real)` | creates the complex number real + 0i |
| `complex(double real, double imaginary)` | creates the complex number real + imaginary*i |
| `double a()` | returns the real part of the number |
| `double b()` | returns the imaginary part of the number |
| `complex conjugate()` | returns the conjugate of the number |
| `bool is_real()` | returns if the complex number is a real number |
| `double magnitude()` | returns the magnitude of the complex vector |
| `double magnitude2()` | returns the magnitude of the complex vector squared |
| `radians direction()` | returns the direction of the number in radians |
| `arcdegrees direction_deg()` | returns the direction of the number in degrees |
| `T convert_if_real(T* buffer)` | converts the number if it is real and stores it in the buffer |

There is also this pow function for raising complex numbers to real numbers

`AUSTINUTILS complex pow(complex z, double n)`

You may also notice that the following function is defined, but it is not currently implemented

`AUSTINUTILS complex pow(complex z, complex w)`

# Other items

`AUSTINUTILS std::vector<std::string> split(std::string s, std::string del = " ")`

**Splits a string at each delimeter string**










