// Program to solve the Peaceful Queens problem
//
// Problem is seen here
// https://www.youtube.com/watch?v=IN1fPtY9jYg

#include <iostream>

#include "puzzle.hpp"

int main() {
    // Do this 1000 times for benchmarking
    int solutions = 0;
    //for (int i=0; i<1000; i++) {
        //std::cout << "=====================================\n";
        //std::cout << "=====================================\n";
        Puzzle first(8);
        solutions = first.solveBoard();
    //}
    std::cout << "Found " << solutions << " solutions\n";
}
