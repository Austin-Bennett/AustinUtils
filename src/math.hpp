#ifndef AUSTINUTILS_MATH_HPP
#define AUSTINUTILS_MATH_HPP

#include <cmath>
#include <functional>
#include <ostream>

#define AUSTINUTILS __declspec(dllexport)

/*
what the math file contains:
    - math functions not already in cmath header
        - sum (start, end, fn), sum(iterable)
        - product (same as sum)
        ect
    - vector2 and vector 3 classes
    - complex number class
    - other misc items
*/

namespace AustinUtils {



    typedef double arcdegrees;
    typedef double radians;

    const double m_PI = 3.14159265358979323846;
    const double m_e = 2.71828182845904523;

    template<typename T>
    concept Iterable = requires(T t) {
        {t.begin()} -> std::same_as<decltype(t.begin())>;
        {t.end()} -> std::same_as<decltype(t.end())>;
    };

    class complex;

    template<typename T>
    concept Arithmetic = std::is_arithmetic_v<T>;


    typedef std::function<double(double)> m_function;

    //functions
    extern AUSTINUTILS double sum(int n, int stop, m_function fn_x);
    extern AUSTINUTILS double product(int n, int stop, m_function fn_x);
    extern AUSTINUTILS arcdegrees toDeg(radians theta);
    extern AUSTINUTILS radians toRad(arcdegrees theta);


    //template functions
    template<Iterable T>
    AUSTINUTILS double sum(T iterable) {
        double ret = 0.0;
        for (auto item: iterable) {
            ret += item;
        }

        return ret;
    }

    template<Iterable T>
    AUSTINUTILS double product(T iterable) {
        double ret = 1.0;
        for (auto item: iterable) {
            ret *= item;
        }

        return ret;
    }

    #define arT template<Arithmetic T>


    arT
    struct AUSTINUTILS v2 {
        T x;
        T y;

        static v2<T> of(radians theta, double magnitude);
        static v2<T> of_deg(arcdegrees theta, double magnitude);
        static v2<double> from(complex c);
        double length();
        double length2();
        radians direction();
        arcdegrees direction_deg();
        v2<T> reversed();
        v2<T> normalized();

        friend std::ostream& operator << (std::ostream& os, v2<T> self) {
            os << "<" << self.x << ", " << self.y << ">";
            return os;
        }



        template<Arithmetic aT>
        v2<aT> convert_data() {
            return {static_cast<aT>(x), static_cast<aT>(y)};
        }

        template<Arithmetic aT>
        double dot(v2<aT> other) {
            return x*other.x + y*other.y;
        }

        template<Arithmetic aT>
        v2<T> operator +(v2<aT> other) {
            return {x + other.x, y + other.y};
        }
        
        template<Arithmetic aT>
        v2<T> operator -(v2<aT> other) {
            return *this + other.reversed();
        }

        v2<T> operator -() {
            return reversed();
        }

        template<Arithmetic aT>
        v2<T> operator *(v2<aT> other) {
            return {x*other.x, y*other.y};
        }

        template<Arithmetic aT>
        v2<T> operator *(aT other) {
            return {x*other, y*other};
        }

        template<Arithmetic aT>
        v2<T> operator /(v2<aT> other) {
            return {x/other.x, y/other.y};
        }

        template<Arithmetic aT>
        v2<T> operator /(aT other) {
            return {x/other, y/other};
        }
        
        //op= variations
        template<Arithmetic aT>
        v2<T>& operator +=(v2<aT> other) {
            *this = *this + other;
            return *this;
        }
        
        template<Arithmetic aT>
        v2<T>& operator -=(v2<aT> other) {
            *this = *this - other;
            return *this;
        }

        template<Arithmetic aT>
        v2<T>& operator *=(v2<aT> other) {
            *this = *this * other;
            return *this;
        }

        template<Arithmetic aT>
        v2<T>& operator *=(aT other) {
            *this = *this * other;
            return *this;
        }

        template<Arithmetic aT>
        v2<T>& operator /=(v2<aT> other) {
            *this = *this / other;
            return *this;
        }

        template<Arithmetic aT>
        v2<T>& operator /=(aT other) {
            *this = *this / other;
            return *this;
        }

    };

    //apparently i cant put these in the cpp file

    arT v2<T> v2<T>::of(radians theta, double magnitude) {
        return {magnitude * std::cos(theta), magnitude * sin(theta)};
    }


    arT v2<T> v2<T>::of_deg(arcdegrees theta, double magnitude) {
        radians rad = toRad(theta);
        return {magnitude * std::cos(rad), magnitude * sin(toRad(theta))};
    }

    arT double v2<T>::length() {
        return sqrt(x*x + y*y);
    }

    arT double v2<T>::length2() {
        return x*x + y*y;
    }

    arT radians v2<T>::direction() {
        return atan(y/x);
    }


    arT arcdegrees v2<T>::direction_deg() {
        return toDeg(atan(y/x));
    }

    arT v2<T> v2<T>::reversed() {
        return {-x, -y};
    }

    arT v2<T> v2<T>::normalized() {
        double invrt = 1.0 / length();
        return {x*invrt, y*invrt};
    }

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

    class AUSTINUTILS complex {
        double Real = 0.0;
        double Imaginary = 0.0;

        public:
        complex();
        complex(double real);
        complex(double real, double imaginary);

        double a();
        double b();
        complex conjugate();
        bool is_real();
        double magnitude();
        double magnitude2();
        radians direction();
        arcdegrees direction_deg();

        arT
        void convert_if_real(T* buffer) {
            if (is_real()) {
                *buffer = Real;
            }
        }

        friend std::ostream& operator <<(std::ostream& os, complex self) {
            if (self.is_real()) {
                os << self.Real;
                return os;
            }
            if (self.Imaginary < 0) {
                os << self.Real << " - " << std::abs(self.Imaginary) << "i";
                return os;
            }
            os << self.Real << " + " << self.Imaginary << "i";
            return os;
        }



        complex operator +(complex other);
        complex operator +(double other);

        complex operator -(complex other);
        complex operator -(double other);
        complex operator -();

        complex operator *(complex other);
        complex operator *(double other);

        complex operator /(complex other);
        complex operator /(double other);

        complex& operator +=(complex other);
        complex& operator +=(double other);

        complex& operator -=(complex other);
        complex& operator -=(double other);

        complex& operator *=(complex other);
        complex& operator *=(double other);

        complex& operator /=(complex other);
        complex& operator /=(double other);

    };



    extern AUSTINUTILS complex pow(complex z, double n);

    extern AUSTINUTILS complex pow(complex z, complex w);

}

#endif