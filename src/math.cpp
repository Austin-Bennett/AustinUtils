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
