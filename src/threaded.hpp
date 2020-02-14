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

        int solveBoard(std::string outDirectory);

        void printBoard();
        void writeToFile(std::ofstream &ofile);
        void saveState();
        void saveState(std::string outDirectory);
    private:
        // Solution static values
        int sideLength;
        int boardSize;
        int startingQueenCount;
        int endingQueenCount;

        std::string baseOutPath;

        // State of the board
        std::vector<int> board;
        std::vector<std::vector<int>> queenPositions;
        int whiteSize;
        int blackSize;
        std::vector<int> rowState;
        std::vector<int> colState;
        std::vector<int> f_diagState;
        std::vector<int> r_diagState;

        int turn; // Whose turn it is
        // Keep track of max queens placed
        int maxQueensPlaced;
        bool rowValid; // Used to jump to next row and reduce checks
        // Number of solutions found
        int solutions;

        // Where to attempt placing the next queen.
        // If placing a queen, next index. If we make it to the end,
        // remove the last placed queen and continue from there
        int startIndex;
        int currentIndex;

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

        // Helper functions for determing max queens
        int getMaxQueensPlaced();

        // Helper functions to output data
        std::string date_toString();
        std::string solution_toString();
        std::string solution_toString(std::vector<std::vector<int>> &vec);

        void convertStringToState(std::fstream& config_file);
};

#endif
