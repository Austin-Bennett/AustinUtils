#include "math.hpp"
#include <cmath>



AUSTINUTILS double AustinUtils::sum(int n, int stop, AustinUtils::m_function fn_x = [](double x) -> double {return x;}) {
    double ret = 0.0;
    for (; n <= stop; n++) {
        ret += fn_x(n);
    }

    return ret;
}

AUSTINUTILS double AustinUtils::product(int n, int stop, AustinUtils::m_function fn_x = [](double x) -> double {return x;}) {
    double ret = 1.0;
    for (; n <= stop; n++) {
        ret *= fn_x(n);
    }

    return ret;
}

AUSTINUTILS AustinUtils::arcdegrees AustinUtils::toDeg(AustinUtils::radians theta) {
    return theta * (180.0 / AustinUtils::m_PI);
}

AUSTINUTILS AustinUtils::radians AustinUtils::toRad(AustinUtils::arcdegrees theta) {
    return theta * (AustinUtils::m_PI/180);
}
//CLASS v2
template<> AustinUtils::v2<double> AustinUtils::v2<double>::from(complex c) {
    return {c.a(), c.b()};
}


//CLASS: complex

AustinUtils::complex::complex() {
    Real = 0;
    Imaginary = 0;
}

AustinUtils::complex::complex(double real) {
    Real = real;
    Imaginary = 0;
}

AustinUtils::complex::complex(double real, double imaginary) {
    Real = real;
    Imaginary = imaginary;
}

double AustinUtils::complex::a() {
    return Real;
}

double AustinUtils::complex::b() {
    return Imaginary;
}

AustinUtils::complex AustinUtils::complex::conjugate() {
    return AustinUtils::complex(Real, -Imaginary);
}

bool AustinUtils::complex::is_real() {
    return Imaginary == 0;
}

double AustinUtils::complex::magnitude() {
    return sqrt(Real*Real + Imaginary*Imaginary);
}

double AustinUtils::complex::magnitude2() {
    return Real*Real + Imaginary*Imaginary;
}

AustinUtils::radians AustinUtils::complex::direction() {
    return atan2(Real, Imaginary);
}

AustinUtils::arcdegrees AustinUtils::complex::direction_deg() {
    return AustinUtils::toDeg(direction());
}

AustinUtils::complex AustinUtils::complex::operator+(double other) {
    return complex(Real + other, Imaginary);
}

AustinUtils::complex AustinUtils::complex::operator+(complex other) {
    return complex(Real + other.a(), Imaginary+other.b());
}

AustinUtils::complex AustinUtils::complex::operator-() {
    return complex(-Real, -Imaginary);
}

AustinUtils::complex AustinUtils::complex::operator-(double other) {
    return *this + (-other);
}
AustinUtils::complex AustinUtils::complex::operator-(complex other) {
    return *this + (-other);
}

AustinUtils::complex AustinUtils::complex::operator*(double other) {
    return complex(Real*other, Imaginary*other);
}

AustinUtils::complex AustinUtils::complex::operator*(complex other) {
    double ac = Real * other.a();
    double bd = Imaginary * other.b();
    double ad = Real * other.b();
    double bc = Imaginary * other.a();

    return complex(ac-bd, ad+bc);
}

AustinUtils::complex AustinUtils::complex::operator/(double other) {
    if (other == 0) {
        return complex(NAN, NAN);
    }
    return {Real / other, Imaginary / other};
}

AustinUtils::complex AustinUtils::complex::operator/(complex other) {
    double ac = Real * other.a();
    double bd = Imaginary * other.b();
    double ad = Real * other.b();
    double bc = Imaginary * other.a();
    return complex(ac+bd, ad-bc) / ((other.a()*other.a()) + (other.b() * other.b()));
}

AustinUtils::complex& AustinUtils::complex::operator+=(double other) {
    *this = *this + other;
    return *this;
}

AustinUtils::complex& AustinUtils::complex::operator+=(complex other) {
    *this = *this + other;
    return *this;
}

AustinUtils::complex& AustinUtils::complex::operator-=(double other) {
    *this = *this - other;
    return *this;
}

AustinUtils::complex& AustinUtils::complex::operator-=(complex other) {
    *this = *this - other;
    return *this;
}

AustinUtils::complex& AustinUtils::complex::operator*=(double other) {
    *this = *this * other;
    return *this;
}

AustinUtils::complex& AustinUtils::complex::operator*=(complex other) {
    *this = *this * other;
    return *this;
}

AustinUtils::complex& AustinUtils::complex::operator/=(double other) {
    *this = *this / other;
    return *this;
}

AustinUtils::complex& AustinUtils::complex::operator/=(complex other) {
    *this = *this / other;
    return *this;
}

AUSTINUTILS AustinUtils::complex pow(AustinUtils::complex z, double N) {
    double rn = pow((z.a() * z.a()) + (z.b() * z.b()), N / 2);
    AustinUtils::radians theta = atan2(z.b(), z.a());
    return AustinUtils::complex(
        rn * cos(N * theta),
        rn * sin(N * theta)
    );
}
