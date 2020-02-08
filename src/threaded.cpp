#include <iostream>
#include <math.h>

#define LIST_USE 0

#include <stdlib.h>
#include <time.h>

#include "threaded.hpp"

Solver::Solver(int sideLength) : Solver(sideLength, 10000, "") {

}
Solver::Solver(int sideLength, int queenStepSize, std::string queenString) :
    sideLength(sideLength),
    boardSize(sideLength*sideLength) {

        // Basic structure setup for now
        queenPositions.push_back(std::vector<int>());
        queenPositions.push_back(std::vector<int>());

        for (int i=0; i<boardSize; i++) {
            board.push_back(EMPTY);
        }

        turn = 0;
        
        for (int i=0; i<sideLength; ++i) {
            rowState.push_back(0);
            colState.push_back(0);
        }
        for (int i=0; i<2*sideLength+1; ++i) {
            f_diagState.push_back(0);
            r_diagState.push_back(0);
        }
    //convertStringToState(queenString);
}

bool Solver::legalPosition(int color, int position) {
    // Checks if a queen of COLOR can be placed at POSITION
    rowError = validRow(color, position);
    if (rowError && color == BLACK && (position % sideLength == position / sideLength) ) {
        return false;
    }
    return rowError
        && validCol (color, position)
        && validDiag(color, position);
}
bool Solver::addQueen(int position) {
    // Determine what color we're placing
    int color = BLACK;
    if (queenPositions[WHITE].size() == queenPositions[BLACK].size()) {
        color = WHITE;
    }

    // Attempt to place queen
    bool canPlace = legalPosition(color, position) &&
                    position <= ( boardSize - maxQueensPlaced + queenPositions[color].size() ) &&
                    position >= 0;

    // Removing permutations of the same solution
    // This is done by limiting the first white piece to be in the
    // upper left quarter of the map, as any other quarter will be
    // either a rotation or a reflection. In the case of odd numbered
    // side lengths, the right border is included and the bottom
    // border is excluded
    if (canPlace) {
        //printBoard();
        if (color == WHITE && queenPositions[WHITE].size() == 0) {
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
        if (queenPositions[BLACK].size() == 0) {
            canPlace = (position % sideLength > position / sideLength);
            if (!canPlace) {
                //std::cout << "C fail\n";
            }
            if (canPlace) {
                canPlace = (position > queenPositions[WHITE][0]);
            }
        }

        // Check that the black piece that is being placed is not
        // on the same row of the first black queen's column, if so, that this
        // queen is not before the first queen
        if (canPlace && queenPositions[BLACK].size() != 0) {
            if ( (position / sideLength == queenPositions[BLACK][0] % sideLength) &&
                    (position % sideLength <  queenPositions[BLACK][0] / sideLength) ) {
                canPlace = false;
            }
        }
    }

    if (canPlace) {
        // Add position to position vector
        queenPositions[color].push_back(position);
        // Update state vectors
        //  White queens are positive
        //  Black queens are negative
        int delta = 1;
        if (color == BLACK) {
            delta = -1;
        }
        rowState[getRowIndex(position)] += delta;
        colState[getColIndex(position)] += delta;
        r_diagState[getReverseDiagIndex(position)] += delta;
        f_diagState[getForwardDiagIndex(position)] += delta;
        // Change the value in the board
        board[position] = color;
    }

    return canPlace;
}
int Solver::removeQueen() {
    // Remove the last queen placed on the board
    int color = BLACK;
    if (queenPositions[WHITE].size() != queenPositions[BLACK].size()) {
        color = WHITE;
    }

    // Check that the board isn't empty
    if (queenPositions[color].size() != 0) {
        int temp = queenPositions[color].back();
        queenPositions[color].pop_back();
        // Remove from board
        board[temp] = EMPTY;
        // Remove from state vectors.
        //  White queens are positive
        //  Black queens are negative
        int delta = 1;
        if (color == BLACK) {
            delta = -1;
        }
        rowState[getRowIndex(temp)] -= delta;
        colState[getColIndex(temp)] -= delta;
        r_diagState[getReverseDiagIndex(temp)] -= delta;
        f_diagState[getForwardDiagIndex(temp)] -= delta;
        // Return position removed
        return temp;
    }
    return -5;
}

//int Solver::solveBoard(std::string outDirectory) {
int Solver::solveBoard() {
    // Attempt to solve the board

    // Where to attempt placing the next queen.
    // If placing a queen, next index. If we make it to the end,
    // remove the last placed queen and continue from there
    int startIndex = 0;
    int currentIndex = 0;

    // Keep track of the highest number of queens we were able to place
    maxQueensPlaced = 0;
    int solutions = 0;

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
            if (queenPositions[turn].size() != 0) {
                currentIndex = queenPositions[turn].back()+1;
            }

            // Check if we just placed black
            // If so, and this is the highest count of queens, print
            if (turn == WHITE) {
                if (queenPositions[WHITE].size() > maxQueensPlaced) {
                    solutions = 0;
                }
                if (queenPositions[WHITE].size() >= maxQueensPlaced) {
                    maxQueensPlaced = queenPositions[WHITE].size();
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
        if (queenPositions[WHITE].size() != 0 && queenPositions[WHITE][0] > startIndex) {
            startIndex = queenPositions[WHITE][0];
        }
    }

    ofile.close();
    return solutions;
}

void Solver::printBoard() {
    time_t currentTime = time(NULL);
    printf("%s", ctime(&currentTime));
    for (int i=0; i<sideLength; i++) {
        for (int j=0; j<sideLength; j++) {
            std::cout << contents(i*sideLength + j);
        }
        std::cout << "\n";
    }
}
void Solver::writeToFile(std::ofstream &ofile) {
    ofile << date_toString();
    ofile << "\n";
    for (int i=0; i<sideLength; i++) {
        for (int j=0; j<sideLength; j++) {
            ofile << contents(i*sideLength + j);
        }
        ofile << "\n";
    }
    ofile << "\n";
}

bool Solver::validPos(int color, int position) {
    // Check that the position in the board is open
    return (board[position] == EMPTY);
}
bool Solver::validRow(int color, int position) {
    // If the value of rowState is negative, there are black queens
    // If the value of rowState is positive, there are white queens

    int row = getRowIndex(position);
    return (color == BLACK && rowState[row] <  1) ||
           (color == WHITE && rowState[row] > -1);
}

bool Solver::validCol(int color, int position) {
    // If the value of rowState is negative, there are black queens
    // If the value of rowState is positive, there are white queens

    int col = getColIndex(position);
    return (color == BLACK && colState[col] <  1) ||
           (color == WHITE && colState[col] > -1);
}

bool Solver::validDiag(int color, int position) {
    // If the value of rowState is negative, there are black queens
    // If the value of rowState is positive, there are white queens

    int fDiag = getForwardDiagIndex(position);
    int rDiag = getReverseDiagIndex(position);

    return ((color == BLACK && f_diagState[fDiag] <  1) ||
            (color == WHITE && f_diagState[fDiag] > -1)   ) &&
           ((color == BLACK && r_diagState[rDiag] <  1) ||
            (color == WHITE && r_diagState[rDiag] > -1)   );
}

int Solver::getRowIndex(int position) {
    return position / sideLength;
}
int Solver::getColIndex(int position) {
    return position % sideLength;
}
int Solver::getForwardDiagIndex(int position) {
    int row = position / sideLength;
    int col = position % sideLength;
    return col - row + sideLength - 1;
}
int Solver::getReverseDiagIndex(int position) {
    int row = position / sideLength;
    int col = position % sideLength;
    return col + row;
}

char Solver::contents(int position) {
    // Check if queen at position.
    switch(board[position]) {
        case BLACK:
            return 'B';
        case WHITE:
            return 'W';
        default:
            return '.';
    }
}

std::string Solver::solution_toString() {
    return solution_toString(queenPositions);
}
std::string Solver::solution_toString(std::vector<std::vector<int>> &vec) {
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

std::string Solver::date_toString() {
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

void Solver::convertStringToState(std::string str) {

}
