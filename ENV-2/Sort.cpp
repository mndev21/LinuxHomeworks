#include "Sort.h"
#include <algorithm>

bool compareComplex(const Complex& a, const Complex& b) {
    return a.abs() < b.abs();
}

void sortComplexNumbers(std::vector<Complex>& arr) {
    std::sort(arr.begin(), arr.end(), compareComplex);
}

