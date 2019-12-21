/*
 * This class handles the current state of the board
 *
 * If data types are changed for memory efficiency,
 * less code will need to be rewritten
 */

#ifndef BOARD_HPP_
#define BOARD_HPP_

class Board {
    public:
        // Contsructors
        Board() = delete;
        Board(uint8_t sidelength);

        // Destructor
        ~Board();

        // Accessors
        uint8_t 

    private:
        uint8_t sideLength;
        uint32_t boardLength;

        uint8_t* board;
};

#endif
