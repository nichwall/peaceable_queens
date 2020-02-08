#ifndef THREADED_HPP
#define THREADED_HPP

#define EMPTY 2
#define WHITE 0
#define BLACK 1

#include <vector>
#include <algorithm>
#include <unordered_set>
#include <fstream>
#include <string.h>

// This is the header for a threaded application of the Peaceable Queens solver

class Solver {
    public:
        Solver(int sideLength);
        Solver(int sideLength, int queenStepSize, std::string queenString);

        bool addQueen(int position);
        int removeQueen();

        int solveBoard();

        void printBoard();
        void writeToFile(std::ofstream &ofile);
    private:
        // Solution static values
        int sideLength;
        int boardSize;
        int startingQueenCount;
        int endingQueenCount;

        // State of the board
        std::vector<int> board;
        std::vector<std::vector<int>> queenPositions;
        std::vector<int> rowState;
        std::vector<int> colState;
        std::vector<int> f_diagState;
        std::vector<int> r_diagState;

        int turn; // Whose turn it is
        int maxQueensPlaced;
        bool rowError; // Used to jump to next row and reduce checks

        // Functions to check whether a position is valid
        bool legalPosition(int color, int position);
        bool validPos(int color, int position);
        bool validRow(int color, int position);
        bool validCol(int color, int position);
        bool validDiag(int color, int position);

        // Indexing into board state position vectors
        int getRowIndex(int position);
        int getColIndex(int position);
        int getForwardDiagIndex(int position);
        int getReverseDiagIndex(int position);

        // Function to get what is in the array
        char contents(int position);

        // Helper functions to output data
        std::string date_toString();
        std::string solution_toString();
        std::string solution_toString(std::vector<std::vector<int>> &vec);

        void convertStringToState(std::string);
};

#endif
