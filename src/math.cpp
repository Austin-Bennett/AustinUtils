#include "math.hpp"
#include <cmath>

AUSTINUTILS double sum(int n, int stop, m_function fn_x = [](double x) -> double {return x;}) {
    double ret = 0.0;
    for (; n <= stop; n++) {
        ret += fn_x(n);
    }

    return ret;
}

AUSTINUTILS double product(int n, int stop, m_function fn_x = [](double x) -> double {return x;}) {
    double ret = 1.0;
    for (; n <= stop; n++) {
        ret *= fn_x(n);
    }

    return ret;
}

AUSTINUTILS arcdegrees toDeg(radians theta) {
    return theta * (180.0 / m_PI);
}

AUSTINUTILS radians toRad(arcdegrees theta) {
    return theta * (m_PI/180);
}
//CLASS v2
template<> v2<double> v2<double>::from(complex c) {
    return {c.a(), c.b()};
}


//CLASS: complex

complex::complex() {
    Real = 0;
    Imaginary = 0;
}

complex::complex(double real) {
    Real = real;
    Imaginary = 0;
}

complex::complex(double real, double imaginary) {
    Real = real;
    Imaginary = imaginary;
}

double complex::a() {
    return Real;
}

double complex::b() {
    return Imaginary;
}

complex complex::conjugate() {
    return complex(Real, -Imaginary);
}

bool complex::is_real() {
    return Imaginary == 0;
}

double complex::magnitude() {
    return sqrt(Real*Real + Imaginary*Imaginary);
}

double complex::magnitude2() {
    return Real*Real + Imaginary*Imaginary;
}

radians complex::direction() {
    return atan2(Real, Imaginary);
}

arcdegrees complex::direction_deg() {
    return toDeg(direction());
}

complex complex::operator+(double other) {
    return complex(Real + other, Imaginary);
}
complex complex::operator+(complex other) {
    return complex(Real + other.a(), Imaginary+other.b());
}

complex complex::operator-() {
    return complex(-Real, -Imaginary);
}

complex complex::operator-(double other) {
    return *this + (-other);
}
complex complex::operator-(complex other) {
    return *this + (-other);
}

complex complex::operator*(double other) {
    return complex(Real*other, Imaginary*other);
}

complex complex::operator*(complex other) {
    double ac = Real * other.a();
    double bd = Imaginary * other.b();
    double ad = Real * other.b();
    double bc = Imaginary * other.a();

    return complex(ac-bd, ad+bc);
}

complex complex::operator/(double other) {
    if (other == 0) {
        return complex(NAN, NAN);
    }
    return {Real / other, Imaginary / other};
}

complex complex::operator/(complex other) {
    double ac = Real * other.a();
    double bd = Imaginary * other.b();
    double ad = Real * other.b();
    double bc = Imaginary * other.a();
    return complex(ac+bd, ad-bc) / ((other.a()*other.a()) + (other.b() * other.b()));
}

complex& complex::operator+=(double other) {
    *this = *this + other;
    return *this;
}

complex& complex::operator+=(complex other) {
    *this = *this + other;
    return *this;
}

complex& complex::operator-=(double other) {
    *this = *this - other;
    return *this;
}

complex& complex::operator-=(complex other) {
    *this = *this - other;
    return *this;
}

complex& complex::operator*=(double other) {
    *this = *this * other;
    return *this;
}

complex& complex::operator*=(complex other) {
    *this = *this * other;
    return *this;
}

complex& complex::operator/=(double other) {
    *this = *this / other;
    return *this;
}

complex& complex::operator/=(complex other) {
    *this = *this / other;
    return *this;
}

AUSTINUTILS complex pow(complex z, double N) {
    double rn = pow((z.a() * z.a()) + (z.b() * z.b()), N / 2);
    radians theta = atan2(z.b(), z.a());
    return complex(
        rn * cos(N * theta),
        rn * sin(N * theta)
    );
}
