#ifndef PUZZLE_HPP_
#define PUZZLE_HPP_

#define WHITE 0
#define BLACK 1

#include <vector>
#include <algorithm>
#include <unordered_set>
#include <fstream>
#include <string.h>

class Puzzle {
    public:
        Puzzle();
        Puzzle(int n);
        ~Puzzle();

        bool legalPosition(int color, int position);

        bool addQueen(int position);
        int removeQueen();

        int solveBoard();

        void printBoard();
    private:
        bool validPos(int color, int position);
        bool validRow(int color, int position);
        bool validCol(int color, int position);
        bool validDiag(int color, int position);

        char contents(int position);

        std::string solution_toString();
        std::string solution_toString(std::vector<std::vector<int>> &vec);

        int sideLength; // Side length of board
        int boardSize;

        std::vector<std::vector<int>> queens;    // Current positions of white and black queens
        int turn;  // 0: white, 1: black

        int maxQueens; // Size to set the queen arrays to

        bool rowError;

        int maxQueensPlaced;
};

#endif
