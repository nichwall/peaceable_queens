#include <iostream>
#include <math.h>

#define LIST_USE 0

#include <stdlib.h>
#include <time.h>

#include "puzzle.hpp"

Puzzle::Puzzle() : Puzzle(5) {
}

Puzzle::Puzzle(int n) {
    this->sideLength = n;
    this->boardSize = n*n;

    this->maxQueens = (n*n)/2; // Arbitrarily set, should match bounds from OEIS

    queens.push_back(std::vector<int>());
    queens.push_back(std::vector<int>());

    for (int i=0; i<boardSize; i++) {
        board.push_back(EMPTY);
    }

    turn = 0;
}

Puzzle::~Puzzle() {
}

bool Puzzle::legalPosition(int color, int position) {
    // Checks if a queen of COLOR can be placed at POSITION
    rowError = validRow(color, position);
    if (rowError && color == BLACK && (position % sideLength == position / sideLength) ) {
        return false;
    }
    return rowError
        && validCol (color, position)
        && validDiag(color, position);
}

bool Puzzle::addQueen(int position) {
    // Determine what color we're placing
    int color = BLACK;
    if (queens[WHITE].size() == queens[BLACK].size()) {
        color = WHITE;
    }

    // Attempt to place queen
    bool canPlace = legalPosition(color, position)
                 && position <= ( boardSize - maxQueensPlaced + queens[color].size() )
                 && position >= 0;

    // Removing permutations of the same solution
    // This is done by limiting the first white piece to be in the
    // upper left quarter of the map, as any other quarter will be
    // either a rotation or a reflection. In the case of odd numbered
    // side lengths, the right border is included and the bottom
    // border is excluded
    if (canPlace) {
        //printBoard();
        if (color == WHITE && queens[WHITE].size() == 0) {
            canPlace = (position % sideLength < ((sideLength+1)/2)
                     && position / sideLength < ((sideLength+1)/2) );
            if (!canPlace) {
                //std::cout << "A fail\n";
            }
        }
    }
    // Check that the black queen is not on the forward diagonal. Any queen that
    // is on the diagonal is a permutation of another solution where white is on the diagonal
    if (canPlace) {
        if (color == BLACK ) {
            canPlace = !(position % sideLength == position / sideLength);
        }
        if (!canPlace) {
            //std::cout << "B fail\n";
        }
    }

    if (canPlace && color == BLACK) {
        // Check if the first black queen is above the diagonal.
        // This should remove flips along the diagonal from the first black
        if (queens[BLACK].size() == 0) {
            canPlace = (position % sideLength > position / sideLength);
            if (!canPlace) {
                //std::cout << "C fail\n";
            }
            // The first black queen also cannot be placed in a position occuring before
            // the first white queen in the top left quarter
            /*
            if (canPlace && position / sideLength < (sideLength+1)/2 &&
                    position % sideLength < (sideLength+1)/2 ) {
                canPlace = (position > queens[WHITE][0]);
                if (!canPlace) {
                    //std::cout << "D fail\n";
                }
            }
            */
            if (canPlace) {
                canPlace = (position > queens[WHITE][0]);
            }
        }

#if 0
        // Check that the black piece being placed would not come before
        // the first black queen if the board is flipped along the diagonal,
        // if being placed below the diagonal
        if (canPlace &&
            position % sideLength < position / sideLength ) {

            int row = position / sideLength;
            int col = position % sideLength;

            int tempIndex = col * sideLength + row;

            if ( tempIndex < queens[BLACK][0] ) {
                std::cout << "Position: " << position << "\tFirst: " << queens[BLACK][0] << "\tTemp: " << tempIndex;
                canPlace = ( board[tempIndex] != EMPTY );
                std::cout << "\tCan place: " << canPlace << "\n";
                if (!canPlace) {
                    std::cout << "E fail\n";
                }
            }
        }
#else
        // Check that the black piece that is being placed is not
        // on the same row of the first black queen's column, if so, that this
        // queen is not before the first queen
        if (canPlace && queens[BLACK].size() != 0) {
            if ( (position / sideLength == queens[BLACK][0] % sideLength) &&
                 (position % sideLength <  queens[BLACK][0] / sideLength) ) {
                canPlace = false;
            }
        }
#endif
    }

    if (canPlace) {
        queens[color].push_back(position);
        // Change the value in the board
        board[position] = color;
    }

    return canPlace;
}
int Puzzle::removeQueen() {
    // Remove the last queen placed on the board
    int color = BLACK;
    if (queens[WHITE].size() != queens[BLACK].size()) {
        color = WHITE;
    }

    if (queens[color].size() != 0) {
        int temp = queens[color].back();
        queens[color].pop_back();
        // Remove from board
        board[temp] = EMPTY;
        return temp;
    } return -5;
}

int Puzzle::solveBoard() {
    // Attempt to solve the board
    
    // Where to attempt placing the next queen.
    // If placing a queen, next index. If we make it to the end,
    // remove the last placed queen and continue from there
    int startIndex = 0;
    int currentIndex = 0;

    // Keep track of the highest number of queens we were able to place
    maxQueensPlaced = 0;
    solutions = 0;

    bool done = false;

    bool lastSuccessful;
    turn = 0;

    //std::ofstream ofile ((date_toString()).c_str());
    std::ofstream ofile ("out.txt");

    //while (startIndex < boardSize-1) {
    while (startIndex < boardSize-1 && currentIndex >= 0) {
        rowError = false;

        // Attempt to place a queen
        lastSuccessful = addQueen(currentIndex);
        if (lastSuccessful) {
            turn = ( turn + 1 ) % 2;

            // If we successfully place a queen, continue from the
            // last position of the other color
            currentIndex = 0;
            if (queens[turn].size() != 0) {
                currentIndex = queens[turn].back()+1;
            }

            // Check if we just placed black
            // If so, and this is the highest count of queens, print
            if (turn == WHITE) {
                if (queens[WHITE].size() > maxQueensPlaced) {
                    solutions = 0;
                }
                if (queens[WHITE].size() >= maxQueensPlaced) {
                    maxQueensPlaced = queens[WHITE].size();
                    solutions++;
#if 1
                    std::cout << "\n";
                    printBoard();
                    writeToFile(ofile);
#endif
                }
            }
        } else {
            // If unsuccessful, attempt next spot
            // Check if we failed on row and should skip to next row
            currentIndex++;
            if (!rowError) {
                currentIndex += (sideLength - currentIndex % sideLength);
            }
            
            // Check if current index is off the board
            if (currentIndex >= boardSize) {
                // If outside of board, remove the last two queens and update
                // indices accordingly
                currentIndex = removeQueen() + 1;
                turn = ( turn + 1 ) % 2;
            }
        }
        // Update the low bound for white queens
        if (queens[WHITE].size() != 0 && queens[WHITE][0] > startIndex) {
            startIndex = queens[WHITE][0];
        }
    }

    ofile.close();
    return solutions;
}

void Puzzle::printBoard() {
    time_t currentTime = time(NULL);
    printf("%s", ctime(&currentTime));
    for (int i=0; i<sideLength; i++) {
        for (int j=0; j<sideLength; j++) {
            std::cout << contents(i*sideLength + j);
        }
        std::cout << "\n";
    }
}

void Puzzle::writeToFile(std::ofstream &ofile) {
    ofile << date_toString();
    ofile << "\n";
    ofile << solutions << "\n";
    for (int i=0; i<sideLength; i++) {
        for (int j=0; j<sideLength; j++) {
            ofile << contents(i*sideLength + j);
        }
        ofile << "\n";
    }
    ofile << "\n";
}

bool Puzzle::validPos(int color, int position) {
    // Loop through all of the other color's queens to check if any of them
    // have the same position as the current position

    int opposing = (color + 1)%2;

    for (int i=0; i<queens[opposing].size(); i++) {
        if (queens[opposing][i] == position) {
            return false;
        }
    }
    for (int i=0; i<queens[color].size(); i++) {
        if (queens[color][i] == position) {
            return false;
        }
    }
    return true;
}
bool Puzzle::validRow(int color, int position) {
    // Loop through all of the other color's queens to check if any of them
    // have the same row as the current position
    
    int opposing = (color + 1)%2;

#if LIST_USE
    for (int i=0; i<queens[opposing].size(); i++) {
        if (queens[opposing][i] / sideLength == position / sideLength) {
            return false;
        }
    }
#else
    int start = position - (position%sideLength);
    for (int i=start; i<start+sideLength; i++) {
        if (board[i] == opposing) {
            return false;
        }
    }
#endif
    return true;
}

bool Puzzle::validCol(int color, int position) {
    // Loop through all of the other color's queens to check if any of them
    // have the same column as the current position

    int opposing = (color + 1)%2;

#if LIST_USE
    for (int i=0; i<queens[opposing].size(); i++) {
        if (queens[opposing][i] % sideLength == position % sideLength) {
            return false;
        }
    }
#else
    int start = position % sideLength;
    for (int i=start; i<boardSize; i+=sideLength) {
        if (board[i] == opposing) {
            return false;
        }
    }
#endif
    return true;
}

bool Puzzle::validDiag(int color, int position) {
    // Loop through all of the other color's queens to check if any of them
    // have the same diagonals

    int opposing = (color + 1)%2;

    int row = position / sideLength;
    int col = position % sideLength;

    int tRow;
    int tCol;

#if LIST_USE
    for (int i=0; i<queens[opposing].size(); i++) {
        tRow = queens[opposing][i] / sideLength;
        tCol = queens[opposing][i] % sideLength;
        
        if (abs(row-tRow) == abs(col-tCol)) {
            return false;
        }
    }
#else
    tRow = row;
    tCol = col;
    while (--tRow >= 0 && --tCol >= 0) {
        if (board[ tRow*sideLength + tCol ] == opposing) {
            return false;
        }
    }
    tRow = row;
    tCol = col;
    while (--tRow >= 0 && ++tCol < sideLength) {
        if (board[ tRow*sideLength + tCol ] == opposing) {
            return false;
        }
    }
    tRow = row;
    tCol = col;
    while (++tRow < sideLength && --tCol >= 0) {
        if (board[ tRow*sideLength + tCol ] == opposing) {
            return false;
        }
    }
    tRow = row;
    tCol = col;
    while (++tRow < sideLength && ++tCol < sideLength) {
        if (board[ tRow*sideLength + tCol ] == opposing) {
            return false;
        }
    }
#endif
    return true;
}

char Puzzle::contents(int position) {
    // Check if queen at position.

    for (int c=0; c<2; c++) {
        for (int i=0; i<maxQueens; i++) {
        }
        for (int i=0; i<queens[c].size(); i++) {
            if (queens[c][i] == position) {
                if (c==WHITE) {
                    return 'W';
                }
                if (c==BLACK) {
                    return 'B';
                }
            }
        }
    }
    return '.';
}

/*
// Mirrors queen locations vertically
void Puzzle::mirror(std::vector<std::vector<int>> &vec, std::vector<std::vector<int>> &out, bool vertical) {
    out = std::vector<std::vector<int>>();
    for (int i=0; i<2; i++) {
        std::vector<int> temp_queens;
        int r1, r2;
        int c1, c2;
        for (int j=0; j<vec[i].size(); j++) {
            r1 = vec[i][j] / sideLength;
            c1 = vec[i][j] % sideLength;

            if (vertical) {
                r2 = sideLength - 1 - r1;
                c2 = c1;
            } else {
                r2 = r1;
                c2 = sideLength - 1 - c1;
            }

            temp_queens.push_back( r2*sideLength + c2 );

            r1 = r2;
            c1 = c2;
        }
        out.push_back(temp_queens);
    }
}

void Puzzle::rotate(std::vector<std::vector<int>> &vec, std::vector<std::vector<int>> &out) {
    for (int i=0; i<2; i++) {
        std::vector<int> temp_queens;
        int r1, r2;
        int c1, c2;
        for (int j=0; j<vec[i].size(); j++) {
            r1 = vec[i][j] / sideLength;
            c1 = vec[i][j] % sideLength;

            r2 = c1;
            c2 = sideLength - 1 - r1;

            temp_queens.push_back( r2*sideLength + c2 );

            r1 = r2;
            c1 = c2;
        }
        out.push_back(temp_queens);
    }
}

void Puzzle::rotations(std::vector<std::vector<int>> &vec, int count) {
    // Rotate count times and put into previous solution maps
    for (int q=0; q<count; q++) {
        std::vector<std::vector<int>> temp_rotation;
        // Rotate
        rotate(vec, temp_rotation);

        // Put rotation into previous solutions
        str_solutions.insert(solution_toString(temp_rotation));
        // Invert rotation
        std::reverse(temp_rotation.begin(), temp_rotation.end());
        // Put inversion into previous solutions
        str_solutions.insert(solution_toString(temp_rotation));
    }

    // Put inversion of vec into previous solutions
    std::reverse(vec.begin(), vec.end());
    str_solutions.insert(solution_toString(vec));
    std::reverse(vec.begin(), vec.end());
}

bool Puzzle::isUnique() {
    //auto got = previousSolutions.find(queens);
    //return (got == previousSolutions.end());
    if (queens[WHITE].size() != 1) return true;
    auto got = str_solutions.find( solution_toString() );

    return (got == str_solutions.end());
}
*/

std::string Puzzle::solution_toString() {
    return solution_toString(queens);
}
std::string Puzzle::solution_toString(std::vector<std::vector<int>> &vec) {
    std::string out = "";
    for (int i=0; i<vec[WHITE].size(); i++) {
        out += vec[WHITE][i] + ",";
    }
    out += "/";
    for (int i=0; i<vec[BLACK].size(); i++) {
        out += vec[BLACK][i] + ",";
    }
    return out;
}

std::string Puzzle::date_toString() {
    time_t t = time(0);

    // Convert to string
    std::string out = ctime(&t);

    // Sanitize string for filename
    for (std::string::iterator it = out.begin(); it != out.end(); ++it) {
        if (*it == ' ')
            *it = '_';
        if (*it == ':')
            *it = '-';
    }

    out.erase(std::remove(out.begin(), out.end(), '\n'), out.end());

    return out;
}
