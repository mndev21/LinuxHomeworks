#include "Complex.h"

Complex::Complex(double real, double imag) : real_(real), imag_(imag) {}

double Complex::real() const {
    return real_;
}

double Complex::imag() const {
    return imag_;
}

Complex Complex::operator+(const Complex& other) const {
    return Complex(real_ + other.real_, imag_ + other.imag_);
}

Complex Complex::operator-(const Complex& other) const {
    return Complex(real_ - other.real_, imag_ - other.imag_);
}

Complex Complex::operator*(double scalar) const {
    return Complex(real_ * scalar, imag_ * scalar);
}

double Complex::abs() const {
    return std::sqrt(real_ * real_ + imag_ * imag_);
}

std::ostream& operator<<(std::ostream& os, const Complex& c) {
    os << "(" << c.real_ << " + " << c.imag_ << "i)";
    return os;
}

