#ifndef AUSTINUTILS_MATH_HPP
#define AUSTINUTILS_MATH_HPP

#include <cmath>
#include <functional>
#include <misc.hpp>
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

    template<typename T>
    concept Integral = std::is_integral_v<T>;

    template<typename T>
    concept FloatingPoint = std::is_floating_point_v<T>;

    template<typename T>
    concept Unsigned = std::is_unsigned_v<T>;




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

    template<Arithmetic T>
    #define ATp template<Arithmetic Tp>
    struct matrix {
        private:
        T** arr = null;
        usize rows;
        usize columns;

        void dealloc() {
            if (arr) {
                if (arr[0]) delete[] arr[0];
                delete[] arr;
                arr = null;

                rows = columns = 0;
            }
        }

        template<Arithmetic Tp>
        static void allocate_arr(Tp**& arr, usize rows, usize columns, Tp default_value = 0) {

            if (rows == 0 || columns == 0) {
                arr = null;
                return;
            }

            arr = new Tp*[rows];

            arr[0] = new Tp[rows*columns];
            for (usize i = 1; i < rows; i++) {
                arr[i] = arr[0] + i * columns;
            }
            for (usize i = 0; i < rows*columns; i++) {
                arr[0][i] = default_value;
            }

        }


        public:

        class matrix_iterator {
            private:
            T* cur;

            public:
            explicit matrix_iterator(T* element_pointer) {
                cur = element_pointer;
            }

            matrix_iterator operator ++(int) {
                matrix_iterator mi = *this;

                ++cur;

                return mi;
            }

            matrix_iterator& operator ++() {
                ++cur;

                return *this;
            }

            matrix_iterator operator --(int) {
                matrix_iterator mi = *this;

                --cur;

                return mi;
            }

            matrix_iterator& operator --() {
                --cur;

                return *this;
            }

            bool operator==(const matrix_iterator &o) const {
                return cur == o.cur;
            }

            bool operator!=(const matrix_iterator &o) const {
                return cur != o.cur;
            }

            T& operator *() {
                return *cur;
            }
        };

        class const_matrix_iterator {
        private:
            const T* cur;

        public:
            explicit const_matrix_iterator(const T* element_pointer) : cur (element_pointer) {

            }

            const_matrix_iterator operator ++(int) {
                const_matrix_iterator mi = *this;

                ++cur;

                return mi;
            }

            const_matrix_iterator& operator ++() {
                ++cur;

                return *this;
            }

            const_matrix_iterator operator --(int) {
                const_matrix_iterator mi = *this;

                --cur;

                return mi;
            }

            const_matrix_iterator& operator --() {
                --cur;

                return *this;
            }

            bool operator==(const const_matrix_iterator &o) const {
                return cur == o.cur;
            }

            bool operator!=(const const_matrix_iterator &o) const {
                return cur != o.cur;
            }

            const T& operator *() const {
                return *cur;
            }
        };

        virtual ~matrix() {
            dealloc();
        }

        usize width() const {
            return columns;
        }

        usize height() const {
            return rows;
        }


        matrix(std::initializer_list<std::initializer_list<T>> matrix) {
            rows = matrix.size();
            if (matrix.begin() != matrix.end()) columns = (*matrix.begin()).size();

            allocate_arr(arr, rows, columns);

            usize i = 0;
            for (const auto& row : matrix) {
                if (row.size() != columns)
                    throw std::runtime_error("Cannot create matrix with non-rectangular initializer list");

                usize j = 0;
                for (const auto& val : row) {
                    arr[i][j] = val;
                    ++j;
                }
                ++i;
            }
        }

        matrix(usize rows, usize columns, T default_value = 0) : rows(rows), columns(columns) {
            allocate_arr(arr, rows, columns, default_value);
        }

        matrix(const matrix& m) : rows(m.rows), columns(m.columns) {
            allocate_arr(arr, rows, columns);
            for (usize i = 0; i < rows; i++) {
                for (usize j = 0; j < columns; j++) {
                    arr[i][j] = m[i][j];
                }
            }
        }

        matrix(matrix&& m) noexcept
            : arr(m.arr), rows(m.rows), columns(m.columns) {
            m.arr = null;
        }

        template<typename iT>
        matrix(iT start, iT end, usize rows, usize columns) : rows(rows), columns(columns) {
            allocate_arr(arr, rows, columns);
            usize i = 0;
            while (start != end && i < rows*columns) {
                arr[0][i] = *start;

                ++i;
                ++start;
            }


        }

        matrix& operator =(const matrix& m) {
            if (&m == this) return *this;

            if (arr != null) dealloc();

            allocate_arr(arr, rows, columns);
            for (usize i = 0; i < rows; i++) {
                for (usize j = 0; j < columns; j++) {
                    arr[i][j] = m[i][j];
                }
            }

            columns = m.columns;
            rows = m.rows;

            return *this;
        }

        matrix& operator=(matrix&& m) noexcept {
            if (this != &m) {
                dealloc();
                rows = m.rows;
                columns = m.columns;
                arr = m.arr;
                m.arr = nullptr;
                m.rows = m.columns = 0;
            }
            return *this;
        }

        T* operator[](usize i) {
            if (i >= rows) {
                throw std::out_of_range("Cannot access element at index " + std::to_string(i));
            }

            return arr[i];
        }

        const T* operator[](usize i) const {
            if (i >= rows) {
                throw std::out_of_range("Cannot access element at index " + std::to_string(i));
            }

            return arr[i];
        }

        matrix_iterator begin() {
            return matrix_iterator(arr[0]);
        }

        matrix_iterator end() {
            return matrix_iterator(arr[0] + rows * columns);
        }

        const_matrix_iterator begin() const {
            return const_matrix_iterator(arr[0]);
        }

        const_matrix_iterator end() const {
            return const_matrix_iterator(arr[0] + rows * columns);
        }

        const_matrix_iterator cbegin() const {
            return const_matrix_iterator(arr[0]);
        }

        const_matrix_iterator cend() const {
            return const_matrix_iterator(arr[0] + rows * columns);
        }

        ATp
        matrix operator +(const matrix<Tp>& other) const {
            if (other.width() != width() || other.height() != height()) {
                throw std::runtime_error("Cannot add matrices of differing dimensions");
            }
            matrix ret = matrix(height(), width());
            for (usize i = 0; i < rows; i++) {
                for (usize j = 0; j < columns; j++) {
                    ret.arr[i][j] = arr[i][j] + other.arr[i][j];
                }
            }

            return ret;
        }

        ATp
        void operator +=(const matrix<Tp>& other) {
            if (other.width() != width() || other.height() != height()) {
                throw std::runtime_error("Cannot add matrices of differing dimensions");
            }
            for (usize i = 0; i < rows; i++) {
                for (usize j = 0; j < columns; j++) {
                    arr[i][j] += other.arr[i][j];
                }
            }
        }

        ATp
        matrix operator -(const matrix<Tp>& other) const {
            if (other.width() != width() || other.height() != height()) {
                throw std::runtime_error("Cannot add matrices of differing dimensions");
            }
            matrix ret = matrix(height(), width());
            for (usize i = 0; i < rows; i++) {
                for (usize j = 0; j < columns; j++) {
                    ret.arr[i][j] = arr[i][j] - other.arr[i][j];
                }
            }

            return ret;
        }

        ATp
        matrix& operator -=(const matrix<Tp>& other) {
            if (other.width() != width() || other.height() != height()) {
                throw std::runtime_error("Cannot add matrices of differing dimensions");
            }
            for (usize i = 0; i < rows; i++) {
                for (usize j = 0; j < columns; j++) {
                    arr[i][j] -= other.arr[i][j];
                }
            }

            return *this;
        }

        template<Arithmetic Tp>
        matrix operator *(Tp scalar) const {
            matrix ret = matrix(*this);
            for (auto& val: ret) {
                val *= scalar;
            }
            return ret;
        }

        template<Arithmetic Tp>
        matrix& operator *=(Tp scalar) {
            for (auto& val: *this) {
                val *= scalar;
            }

            return *this;
        }

        ATp
        matrix operator *(const matrix<Tp>& mat) const {
            if (width() != mat.height()) {
                throw std::runtime_error("Can only multiply m*n matrix with matrix of n*p dimensions");
            }

            matrix result(height(), mat.width());

            for (usize i = 0; i < height(); i++) {
                for (usize j = 0; j < mat.width(); j++) {
                    result.arr[i][j] = 0;
                    for (usize k = 0; k < width(); k++) {
                        result.arr[i][j] += arr[i][k] * mat.arr[k][j];
                    }
                }
            }

            return result;
        }

        ATp
        matrix& operator *=(const matrix<Tp>& mat) {
            if (width() != mat.height()) {
                throw std::runtime_error("Can only multiply m*n matrix with matrix of n*p dimensions");
            }

            *this = std::move((*this) * mat);


            return *this;
        }

        friend std::ostream& operator << (std::ostream& os, const matrix& self) {
            os << "[";
            for (usize i = 0; i < self.rows; i++) {
                os << "[";
                for (usize j = 0; j < self.columns; j++) {
                    if (j != self.columns-1) os << self[i][j] << ", ";
                    else os << self[i][j];
                }
                if (i != self.rows-1) os << "],\n";
                else os << "]";
            }
            os << "]";

            return os;
        }

    };

}

#endif