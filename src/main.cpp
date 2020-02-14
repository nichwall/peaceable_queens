// Program to solve the Peaceful Queens problem
//
// Problem is seen here
// https://www.youtube.com/watch?v=IN1fPtY9jYg

#include <iostream>

//#include "puzzle.hpp"
#include "threaded.hpp"

int main(int argc, char** argv) {
    int solutions = 0;

#if 1
    Solver first(std::stoi(argv[1]), std::stoi(argv[2]), argv[3]);
    solutions = first.solveBoard("workingDirectory");
    std::cout << "Found " << solutions << " solutions\n";
#else
    //Solver second(5, 1, "workingDirectory/config1.txt");
    Solver second(5, 1, "workingDirectory/0-2-10-12-_9-19-21-23-.txt");
    second.printBoard();

    std::cout << "Attempting solve from state.\n";
    second.solveBoard("workingDirectory");
#endif
}
