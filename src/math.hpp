#ifndef AUSTINUTILS_MATH_HPP
#define AUSTINUTILS_MATH_HPP

#include <cmath>
#include <functional>
#include <misc.hpp>
#include <ostream>


#define AUSTINUTILS __declspec(dllexport)


namespace AustinUtils {

    class str;
    str operator ""_str(const char*, usize);

    typedef double arcdegrees;
    typedef double radians;

    const double m_PI = 3.14159265358979323846;
    const double m_e = 2.71828182845904523;



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

    template<Arithmetic T, Arithmetic... Args, typename = std::enable_if_t<std::conjunction_v<std::is_same<Args, T>...>>>
    T max(Args... args) {
        T ret = T_MIN(T);
        ((ret = args > ret ? args:ret), ...);
        return ret;
    }

    template<Arithmetic T, Arithmetic... Args, typename = std::enable_if_t<std::conjunction_v<std::is_same<Args, T>...>>>
    T min(Args... args) {
        T ret = T_MAX(T);
        ((ret = args < ret ? args:ret), ...);
        return ret;
    }

    #define arT template<Arithmetic T>
    arT
    struct AUSTINUTILS v2 {
        T x = 0;
        T y = 0;

        static v2<T> of(radians theta, double magnitude);
        static v2<T> of_deg(arcdegrees theta, double magnitude);
        static v2<double> from(complex c);
        double length();
        double length2();
        radians direction();
        arcdegrees direction_deg();
        v2<T> reversed();
        v2<T> &reverse() {
            this.x *= -1;
            this.y *= -1;
            return *this;
        }
        v2<T> normalized();
        v2<T> &normalize() {
            double invroot = 1/length();
            x *= invroot;
            y *= invroot;
            return *this;
        }

        friend std::ostream& operator << (std::ostream& os, v2<T> self) {
            os << "<" << self.x << ", " << self.y << ">";
            return os;
        }



        template<Arithmetic aT>
        v2<aT> convert_data() {
            return {static_cast<aT>(x), static_cast<aT>(y)};
        }

        template<Arithmetic aT>
        double dot(v2<aT> other) const {
            return x*other.x + y*other.y;
        }

        template<Arithmetic aT>
        v2<T> operator +(v2<aT> other) const {
            return {x + other.x, y + other.y};
        }
        
        template<Arithmetic aT>
        v2<T> operator -(v2<aT> other) const {
            return *this + other.reversed();
        }

        v2<T> operator -() const {
            return reversed();
        }

        template<Arithmetic aT>
        v2<T> operator *(v2<aT> other) const {
            return {x*other.x, y*other.y};
        }

        template<Arithmetic aT>
        v2<T> operator *(aT other) const {
            return {x*other, y*other};
        }

        template<Arithmetic aT>
        v2<T> operator /(v2<aT> other) const {
            return {x/other.x, y/other.y};
        }

        template<Arithmetic aT>
        v2<T> operator /(aT other) const {
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

        v2 rotated(radians theta, v2 center = {0, 0}) {
            v2 ret = *this;
            ret -= center;

            ret.x = ret.x*std::cos(theta) - ret.y*std::sin(theta);
            ret.y = ret.x*std::sin(theta) + ret.y*std::cos(theta);
            ret += center;
            return ret;
        }

        v2 rotated_deg(arcdegrees theta, v2 center = {0, 0}) {
            return rotated(toRad(theta), center);
        }

        v2& rotate(radians theta, v2 center = {0, 0}) {
            this->operator-=(center);
            x = x*std::cos(theta) - y*std::sin(theta);
            y = x*std::sin(theta) + y*std::cos(theta);
            this->operator+=(center);

            return *this;
        }

        v2& rotate_deg(arcdegrees theta, v2 center = {0, 0}) {
            return rotate(toRad(theta, center));
        }

        template<Arithmetic Tp>
        bool operator==(const v2<Tp> &o) const {
            return x == o.x && y == o.y;
        }

        template<Arithmetic Tp>
        bool operator !=(const v2<Tp>& o) const {
            return !(*this == o);
        }

        str toStr();

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

    arT
    struct AUSTINUTILS v3 : public v2<T> {
        using v2<T>::x;
        using v2<T>::y;
        T z = 0;



        static v3 of(T magnitude, radians xrot, radians yrot, radians zrot) {
            v3 ret = {magnitude, 0, 0};
            ret.rotate(xrot, yrot, zrot);


            return ret;
        }

        static v3 of_deg(T magnitude, arcdegrees xrot, arcdegrees yrot, arcdegrees zrot) {
            return of(magnitude, toRad(xrot), toRad(yrot), toRad(zrot));
        }

        static v3 of(T magnitude, v3<radians> v) {
            return of(magnitude, v.x, v.y, v.z);
        }

        static v3 of_deg(T magnitude, v3<arcdegrees> v) {
            return of_deg(magnitude, v.x, v.y, v.z);
        }

        template<typename Tp>
        v3<Tp> convert_data() {
            return {cast(x, Tp), cast(y, Tp), cast(z, Tp)};
        }

        v3<radians> direction() {
            v3<radians> ret = {};
            ret.z = atan2(y, x);
            ret.y = atan2(z, x);
            ret.x = atan2(z, y);
            return ret;
        }

        v3<arcdegrees> direction_deg() {
            v3<arcdegrees> ret = {};
            ret.z = toDeg(atan2(y, x));
            ret.y = toDeg(atan2(z, x));
            ret.x = toDeg(atan2(z, y));

            return ret;
        }

        T dot(const v3& other) {
            return x*other.x + y*other.y + z*other.z;
        }

        T length() {
            return sqrt(x*x + y*y + z*z);
        }

        T length2() {
            return x*x + y*y + z*z;
        }

        v3 normalized() {
            double invrt = 1/length();
            v3 ret = {x*invrt, y*invrt, z*invrt};
            return ret;
        }

        v3& normalize() {
            double invrt = 1/length();
            x *= invrt;
            y *= invrt;
            z *= invrt;
            return *this;
        }

        v3 reversed() {
            return {-x, -y, -z};
        }

        v3 &reverse() {
            x*=-1;
            y*=-1;
            z*=-1;
            return *this;
        }

        v3 crossed(const v3& other) {
            return {
                y*other.z - z*other.y,
                z*other.x - x*other.z,
                x*other.y - y*other.x
            };
        }

        v3 &cross(const v3& other) {
            x = y*other.z - z*other.y;
            y = z*other.x - x*other.z;
            z = x*other.y - y*other.x;
            return *this;
        }

        v3 rotated(radians xrot, radians yrot, radians zrot, v3 center = {0, 0, 0}) {
            v3 ret = *this;

            // Step 1: Translate vector to the origin by subtracting the center
            ret -= center;

            // Step 2: Rotate around the Z-axis (roll)
            float cos_z = cos(zrot);
            float sin_z = sin(zrot);
            float temp_x = ret.x * cos_z - ret.y * sin_z;
            float temp_y = ret.x * sin_z + ret.y * cos_z;
            ret.x = temp_x;
            ret.y = temp_y;

            // Step 3: Rotate around the Y-axis (yaw)
            float cos_y = cos(yrot);
            float sin_y = sin(yrot);
            float temp_z = ret.z * cos_y - ret.x * sin_y;
            ret.x = ret.x * cos_y + ret.z * sin_y;
            ret.z = temp_z;

            // Step 4: Rotate around the X-axis (pitch)
            float cos_x = cos(xrot);
            float sin_x = sin(xrot);
            temp_y = ret.y * cos_x - ret.z * sin_x;
            ret.z = ret.y * sin_x + ret.z * cos_x;
            ret.y = temp_y;

            // Step 5: Translate back to the original position
            ret += center;

            return ret;
        }

        v3 rotated(v3<radians> rotator, v3 center) {
            return rotated(rotator.x, rotator.y, rotator.z, center);
        }

        v3 rotated_deg(arcdegrees xrot, arcdegrees yrot, arcdegrees zrot, v3 center = {0, 0, 0}) {
            return rotated(toRad(xrot), toRad(yrot), toRad(zrot));
        }

        v3 rotated_deg(v3<arcdegrees> rotator, v3 center) {
            return rotated_deg(rotator.x, rotator.y, rotator.z, center);
        }

        v3& rotate(radians xrot, radians yrot, radians zrot, v3 center = {0, 0, 0}) {
            *this = rotated(xrot, yrot, zrot, center);
            return *this;
        }

        v3& rotate(v3<radians> rotator, v3 center) {
            return rotate(rotator.x, rotator.y, rotator.z, center);
        }

        v3& rotate_deg(arcdegrees xrot, arcdegrees yrot, arcdegrees zrot, v3 center = {0, 0, 0}) {
            return rotate(toRad(xrot), toRad(yrot), toRad(zrot), center);
        }

        v3 rotate_deg(v3<radians> rotator, v3 center) {
            return rotate_deg(rotator.x, rotator.y, rotator.z, center);
        }

        template<Arithmetic Tp>
        v3 operator +(const v3<Tp>& o) const {
            return {x+o.x, y + o.y, z+o.z};
        }

        template<Arithmetic Tp>
        v3 operator -(const v3<Tp>& o) const {
            return {x-o.x, y-o.y, z-o.z};
        }

        template<Arithmetic Tp>
        v3 operator *(const v3<Tp>& o) const {
            return {x*o.x, y*o.y, z*o.z};
        }

        template<Arithmetic Tp>
        v3 operator *(const Tp o) const {
            return {x*o, y*o, z*o};
        }

        template<Arithmetic Tp>
        v3 operator /(const v3<Tp>& o) const {
            return {x/o.x, y/o.y, z/o.z};
        }

        template<Arithmetic Tp>
        v3 operator /(const Tp o) const {
            return {x/o, y/o, z/o};
        }

        template<Arithmetic Tp>
        v3 &operator +=(const v3<Tp>& o) {
            x += o.x;
            y += o.y;
            z += o.z;

            return *this;
        }

        template<Arithmetic Tp>
        v3 &operator -=(const v3<Tp>& o) {
            x -= o.x;
            y -= o.y;
            z -= o.z;

            return *this;
        }

        template<Arithmetic Tp>
        v3 &operator *=(const v3<Tp>& o) {
            x *= o.x;
            y *= o.y;
            z *= o.z;

            return *this;
        }

        template<Arithmetic Tp>
        v3 &operator *=(const Tp o) {
            x *= o;
            y *= o;
            z *= o;

            return *this;
        }

        template<Arithmetic Tp>
        v3 &operator /=(const v3<Tp>& o) {
            x /= o.x;
            y /= o.y;
            z /= o.z;

            return *this;
        }

        template<Arithmetic Tp>
        v3 &operator /=(const Tp o) {
            x /= o;
            y /= o;
            z /= o;

            return *this;
        }



        template<Arithmetic aT>
        v3<T> operator +(v2<aT> other) const {
            return {x + other.x, y + other.y, z};
        }

        template<Arithmetic aT>
        v3<T> operator -(v2<aT> other) const {
            return *this + other.reversed();
        }

        v3<T> operator -() const {
            return reversed();
        }

        template<Arithmetic aT>
        v3<T> operator *(v2<aT> other) const {
            return {x*other.x, y*other.y, z};
        }


        template<Arithmetic aT>
        v3<T> operator /(v2<aT> other) const {
            return {x/other.x, y/other.y, z};
        }



        //op= variations
        template<Arithmetic aT>
        v3<T>& operator +=(v2<aT> other) {
            *this = *this + other;
            return *this;
        }

        template<Arithmetic aT>
        v3<T>& operator -=(v2<aT> other) {
            *this = *this - other;
            return *this;
        }

        template<Arithmetic aT>
        v3<T>& operator *=(v2<aT> other) {
            *this = *this * other;
            return *this;
        }

        template<Arithmetic aT>
        v3<T>& operator /=(v2<aT> other) {
            *this = *this / other;
            return *this;
        }

        template<Arithmetic Tp>
        bool operator==(const v2<Tp> &o) const {
            return x == o.x && y == o.y;
        }

        template<Arithmetic Tp>
        bool operator !=(const v2<Tp>& o) const {
            return !(*this == o);
        }

        template<Arithmetic Tp>
        bool operator ==(const v3<Tp>& o) const {
            return x == o.x && y == o.y && z == o.z;
        }

        template<Arithmetic Tp>
        bool operator !=(const v3<Tp>& o) const {
            return x != o.x || y != o.y || z != o.z;
        }

        friend std::ostream& operator <<(std::ostream& os, const v3& self) {
            os << "<" << self.x << ", " << self.y << ", " << self.z << ">";

            return os;
        }

        str toStr();
    };

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


    class AUSTINUTILS complex {
    public:
        double a = 0.0;
        double b = 0.0;


        complex();
        explicit complex(double real);
        complex(double real, double imaginary);


        complex conjugate() const;
        bool is_real() const;
        double magnitude() const;
        double magnitude2() const;
        radians direction() const;
        arcdegrees direction_deg() const;

        arT
        void convert_if_real(T* buffer) {
            if (is_real()) {
                *buffer = a;
            }
        }

        friend std::ostream& operator <<(std::ostream& os, complex self) {
            if (self.is_real()) {
                os << self.a;
                return os;
            }
            if (self.b < 0) {
                os << self.a << " - " << std::abs(self.b) << "i";
                return os;
            }
            os << self.a << " + " << self.b << "i";
            return os;
        }



        complex operator +(complex other) const;
        complex operator +(double other) const;

        complex operator -(complex other) const;
        complex operator -(double other) const;
        complex operator -() const;

        complex operator *(complex other) const;
        complex operator *(double other) const;

        complex operator /(complex other) const;
        complex operator /(double other) const;

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