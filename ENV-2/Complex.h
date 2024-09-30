#ifndef COMPLEX_H
#define COMPLEX_H

#include <iostream>
#include <cmath>

class Complex {
public:
    Complex(double real = 0.0, double imag = 0.0);

    double real() const;
    double imag() const;

    Complex operator+(const Complex& other) const;
    Complex operator-(const Complex& other) const;
    Complex operator*(double scalar) const;
    double abs() const;

    friend std::ostream& operator<<(std::ostream& os, const Complex& c);

private:
    double real_;
    double imag_;
};

#endif // COMPLEX_H

