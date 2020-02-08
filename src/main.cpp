// Program to solve the Peaceful Queens problem
//
// Problem is seen here
// https://www.youtube.com/watch?v=IN1fPtY9jYg

#include <iostream>

//#include "puzzle.hpp"
#include "threaded.hpp"

int main() {
    int solutions = 0;

    Solver first(8);
    solutions = first.solveBoard();

    std::cout << "Found " << solutions << " solutions\n";
}
