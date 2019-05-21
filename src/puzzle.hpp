#ifndef PUZZLE_HPP_
#define PUZZLE_HPP_

#define WHITE 0
#define BLACK 1

#include <vector>
#include <algorithm>
#include <unordered_set>
#include <fstream>
#include <string.h>

// BEGIN
// Pulled from StackOverflow for hash of vector. Required for unordered_set

//using boost::hash_combine
    template <class T>
inline void hash_combine(std::size_t& seed, T const& v)
{
    seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}
namespace std
{
    template<typename T>
    struct hash<vector<T>>
    {
        typedef vector<T> argument_type;
        typedef std::size_t result_type;
        result_type operator()(argument_type const& in) const
        {
            size_t size = in.size();
            size_t seed = 0;
            for (size_t i = 0; i < size; i++)
                //Combine the hash of the current vector with the hashes of the previous ones
                hash_combine(seed, in[i]);
            return seed;
        }
    };
}
// END

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

        void insertIntoSolution();
        bool isUnique();

        void rotations(std::vector<std::vector<int>> &vec, int count);
        void rotate(std::vector<std::vector<int>> &vec, std::vector<std::vector<int>> &out);
        void mirror(std::vector<std::vector<int>> &vec, std::vector<std::vector<int>> &out, bool vertical);

        std::string solution_toString();
        std::string solution_toString(std::vector<std::vector<int>> &vec);

        int sideLength; // Side length of board
        int boardSize;

        std::vector<std::vector<int>> queens;    // Current positions of white and black queens
        int turn;  // 0: white, 1: black

        int maxQueens; // Size to set the queen arrays to

        // Vector of linked lists, with previous solutions contained in the
        // linked lists. The outermost vector is how many queens are in the
        // solution.
        std::unordered_set< std::vector<std::vector<int>> > previousSolutions;
        std::unordered_set< std::string > str_solutions;

        bool rowError;
};

#endif
