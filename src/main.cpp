// Program to solve the Peaceful Queens problem
//
// Problem is seen here
// https://www.youtube.com/watch?v=IN1fPtY9jYg

#include <iostream>

#include "puzzle.hpp"

int main() {
    // Do this 1000 times for benchmarking
    int solutions = 0;
    //for (int i=0; i<100; i++) {
        //std::cout << "=====================================\n";
        //std::cout << "=====================================\n";
        Puzzle first(7);
        solutions = first.solveBoard();
    //}
    std::cout << "Found " << solutions << " solutions\n";
}
