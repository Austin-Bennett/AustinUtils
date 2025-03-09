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
    return {c.a, c.b};
}




//CLASS: complex

AustinUtils::complex::complex() {
    a = 0;
    b = 0;
}

AustinUtils::complex::complex(double real) {
    a = real;
    b = 0;
}

AustinUtils::complex::complex(double real, double imaginary) {
    a = real;
    b = imaginary;
}






AustinUtils::complex AustinUtils::complex::conjugate() const {
    return {a, -b};
}

bool AustinUtils::complex::is_real() const {
    return b == 0;
}

double AustinUtils::complex::magnitude() const {
    return sqrt(a*a + b*b);
}

double AustinUtils::complex::magnitude2() const {
    return a*a + b*b;
}

AustinUtils::radians AustinUtils::complex::direction() const {
    return atan2(a, b);
}

AustinUtils::arcdegrees AustinUtils::complex::direction_deg() const {
    return AustinUtils::toDeg(direction());
}

AustinUtils::complex AustinUtils::complex::operator+(double other) const {
    return complex(a + other, b);
}

AustinUtils::complex AustinUtils::complex::operator+(complex other) const {
    return complex(a + other.a, b+other.b);
}

AustinUtils::complex AustinUtils::complex::operator-() const {
    return {-a, -b};
}

AustinUtils::complex AustinUtils::complex::operator-(double other) const {
    return *this + (-other);
}
AustinUtils::complex AustinUtils::complex::operator-(complex other)  const{
    return *this + (-other);
}

AustinUtils::complex AustinUtils::complex::operator*(double other) const {
    return complex(a*other, b*other);
}

AustinUtils::complex AustinUtils::complex::operator*(complex other) const {
    double ac = a * other.a;
    double bd = b * other.b;
    double ad = a * other.b;
    double bc = b * other.a;

    return complex(ac-bd, ad+bc);
}

AustinUtils::complex AustinUtils::complex::operator/(double other) const {
    if (other == 0) {
        return complex(NAN, NAN);
    }
    return {a / other, b / other};
}

AustinUtils::complex AustinUtils::complex::operator/(complex other) const {
    double ac = a * other.a;
    double bd = b * other.b;
    double ad = a * other.b;
    double bc = b * other.a;
    return complex(ac+bd, ad-bc) / ((other.a*other.a) + (other.b * other.b));
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
    double rn = pow((z.a * z.a) + (z.b * z.b), N / 2);
    AustinUtils::radians theta = atan2(z.b, z.a);
    return AustinUtils::complex(
        rn * cos(N * theta),
        rn * sin(N * theta)
    );
}
