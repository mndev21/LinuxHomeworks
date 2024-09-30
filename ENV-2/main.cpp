#include <iostream>
#include <vector>
#include "Complex.h"
#include "Sort.h"

int main() {
    std::vector<Complex> complexNumbers = {
        Complex(3.0, 4.0),
        Complex(1.0, 1.0),
        Complex(2.0, 3.0),
        Complex(0.0, 2.0),
        Complex(5.0, 1.0)
    };

    std::cout << "Complex numbers before sorting:" << std::endl;
    for (const auto& c : complexNumbers) {
        std::cout << c << " ";
    }
    std::cout << std::endl;

    sortComplexNumbers(complexNumbers);

    std::cout << "Complex numbers after sorting (by absolute value):" << std::endl;
    for (const auto& c : complexNumbers) {
        std::cout << c << " ";
    }
    std::cout << std::endl;

    return 0;
}

