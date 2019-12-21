// This file tests whether diagonal selection for vectors is correct

#include <iostream>
#include <iomanip>

uint32_t sideLength = 5;
uint32_t arrayLength = sideLength * sideLength;

int forwardDiag(int val) {
    int row = val / sideLength;
    int col = val % sideLength;

    int diff = col - row + sideLength - 1;
    return diff;
}
int reverseDiag(int val) {
    int row = val / sideLength;
    int col = val % sideLength;

    int diff = col + row;
    return diff;
}

void test(int* mat, int func(int)) {
    // Set values to -1
    for (uint32_t i=0; i<arrayLength; ++i) {
        mat[i] = -1;
    }

    // Get diagonals
    for (uint32_t i=0; i<arrayLength; ++i) {
        mat[i] = func(i);
    }

    // Print matrix
    for (uint32_t i=0; i<sideLength; ++i) {
        for (uint32_t j=0; j<sideLength; ++j) {
            std::cout << std::setw(4) << mat[i*sideLength + j];
        }
        std::cout << "\n";
    }
    std::cout << "\n\n";
}

int main() {
    // Initialize
    int diags[arrayLength];

    // Get forward diagonal of matrix 
    test(diags, forwardDiag);
    // Get reverse diagonal of matrix 
    test(diags, reverseDiag);
}
