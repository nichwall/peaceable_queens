/*
 * This class handles each state to be worked on
 */

#ifndef STATE_HPP_
#define STATE_HPP_

#include <vector>

// Color defines
#define WHITE 1
#define BLACK 2

template <class T>
class State {
    public:
        // Constructors
        State();

        // Destructors

    private:
        // Queen position vectors
        std::vector<T> whitePositions;
        std::vector<T> blackPositions;

        // Board state
        std::vector<

        // Line flags
        std::vector< std::vector<T> > flags;
        // Bit offset for flags
        T var_size;

        // Minimum queen count
        T minimum_queens;
        // Max queen count
        T maximum_queens;
};

#endif
