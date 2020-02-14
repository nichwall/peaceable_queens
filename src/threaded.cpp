#include <iostream>
#include <math.h>

#define THIS_SPEEDUP 0

#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <filesystem>

#include "threaded.hpp"

Solver::Solver(int sideLength) : Solver(sideLength, 10000, "") {

}
Solver::Solver(int sideLength, int queenStepSize, std::string inputFile) :
    sideLength(sideLength),
    boardSize(sideLength*sideLength) {

        // Basic structure setup for now
        queenPositions.push_back(std::vector<int>());
        queenPositions.push_back(std::vector<int>());
        whiteSize = 0;
        blackSize = 0;

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
        startIndex = 0;
        currentIndex = 0;
        solutions = 0;
        startingQueenCount = 0;
        // Determine the maximum number of queens that have been placed
        maxQueensPlaced = 0;

        // Update status based on config file
        if (inputFile.size() != 0) {
            std::fstream configFile(inputFile, std::fstream::in);
            convertStringToState(configFile);
            configFile.close();
        }
        endingQueenCount = startingQueenCount + queenStepSize;
}

bool Solver::legalPosition(int color, int position) {
    // Black queens cannot be on the diagonal because this causes an inversion
    // of a White queen on the diagonal.

    // Checks if a queen of COLOR can be placed at POSITION
    rowValid = true;
    return validRow (color, position)
        && (color != BLACK || (position % sideLength != position / sideLength))
        && validCol (color, position)
        && validDiag(color, position);
}
bool Solver::addQueen(int position) {
    // Determine what color we're placing
    int color = BLACK;

    if (whiteSize == blackSize) {
        color = WHITE;
    }

    // Attempt to place queen
    int queenSize = (color==WHITE) ? whiteSize : blackSize;
    bool canPlace = position < ( boardSize - maxQueensPlaced + queenSize + 1) &&
                    position >= 0 &&
                    queenSize < endingQueenCount && 
                    legalPosition(color, position);

    // Removing permutations of the same solution
    // This is done by limiting the first white piece to be in the
    // upper left quarter of the map, as any other quarter will be
    // either a rotation or a reflection. In the case of odd numbered
    // side lengths, the right border is included and the bottom
    // border is excluded
    int row = getRowIndex(position);
    int col = getColIndex(position);
#if 0
    canPlace = canPlace &&
            (
                color != WHITE ||
                whiteSize != 0 ||
                row < ((sideLength+1)/2) ||
                col < ((sideLength+1)/2)
            );
#else
    if (canPlace) {
        if (color == WHITE && whiteSize == 0) {
            canPlace = (position % sideLength < ((sideLength+1)/2)
                     && position / sideLength < ((sideLength+1)/2) );
        }
    }
#endif

    if (canPlace && color == BLACK) {
        // Check if the first black queen is above the diagonal.
        // This should remove flips along the diagonal from the first black
        if (blackSize == 0) {
            canPlace = (col > row) &&
                       (position > queenPositions[WHITE][0]);
        }

        // Check that the black piece that is being placed is not
        // on the same row of the first black queen's column, if so, that this
        // queen is not before the first queen
        if (canPlace && blackSize != 0) {
#if 0
            int temp = queenPositions[BLACK][0];
            if ( (position / sideLength == temp % sideLength) &&
                    (position % sideLength <  temp / sideLength) ) {
                canPlace = false;
            }
#else
            if ( (position / sideLength == queenPositions[BLACK][0] % sideLength) &&
                    (position % sideLength <  queenPositions[BLACK][0] / sideLength) ) {
                canPlace = false;
            }
#endif
        }
    }

    if (canPlace) {
        // Add position to position vector
        queenPositions[color].push_back(position);
        if (color == WHITE) {
            whiteSize++;
        } else {
            blackSize++;
        }
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
    if (whiteSize != blackSize) {
        color = WHITE;
    }

    // Check that the board isn't empty
    if ((color == WHITE && whiteSize != 0) || (color == BLACK && blackSize != 0)) {
        int temp = queenPositions[color].back();
        queenPositions[color].pop_back();
        if (color == WHITE) {
            whiteSize--;
        } else {
            blackSize--;
        }
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

int Solver::solveBoard(std::string outDirectory) {
//int Solver::solveBoard() {
    // Attempt to solve the board

    baseOutPath = outDirectory;

    bool done = false;

    bool lastSuccessful;
    turn = WHITE;

    //std::ofstream ofile ((date_toString()).c_str());
    std::ofstream ofile ("out.txt");

    while (startIndex < boardSize-1 && currentIndex >= 0) {
        rowValid = true;

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
                if (whiteSize > maxQueensPlaced) {
                    solutions = 0;
                }
                if (whiteSize >= maxQueensPlaced) {
                    maxQueensPlaced = whiteSize;
                    solutions++;
#if 1
                    std::cout << "\n";
                    printBoard();
                    writeToFile(ofile);
#endif
                }
#if 0
                // Check if we have reached the cutoff for this run
                if (queenPositions[WHITE].size() == endingQueenCount) {
                    saveState();
                }
                // If we have not reached the cutoff, but are higher
                // than maxQueens, double check whether to save, then
                // save is necessary.
                else if (queenPositions[WHITE].size() >= maxQueensPlaced){
                    maxQueensPlaced = getMaxQueensPlaced();
                    if (queenPositions[WHITE].size() >= maxQueensPlaced) {
                        saveState();
                    }
                }
#endif
            }
        } else {
            // If unsuccessful, attempt next spot
            // Check if we failed on row and should skip to next row
            currentIndex++;
            if (!rowValid) {
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
        if (whiteSize != 0 && queenPositions[WHITE][0] > startIndex) {
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
void Solver::saveState() {
    saveState(baseOutPath);
}
void Solver::saveState(std::string outDirectory) {
    std::ostringstream temp;
    // Convert queen positions into unique identifier
    std::copy(queenPositions[WHITE].begin(), queenPositions[WHITE].end(), 
        std::ostream_iterator<int>(temp, "-")); 
    temp << "_";
    std::copy(queenPositions[BLACK].begin(), queenPositions[BLACK].end(), 
            std::ostream_iterator<int>(temp, "-")); 
  
    // Convert from stringstream to std::string
    std::string identifier = temp.str();

    // Clear string stream for output data
    temp.str(std::string());

    // Write side length
    temp << sideLength << std::endl;
    // Write queen count
    temp << whiteSize << std::endl;
    // Write queen positions
    for (size_t i=0; i<whiteSize; ++i) {
        temp << queenPositions[WHITE][i] << " ";
        temp << queenPositions[BLACK][i] << std::endl;
    }

    // Convert from stringstream to std::string
    std::string outData = temp.str();

    // Open solutions file for writing
    std::fstream outFile( (outDirectory+"/"+std::to_string(queenPositions[WHITE].size())+"/"+identifier+".txt").c_str(), std::fstream::out);
    outFile << outData;
    outFile.close();
}

bool Solver::validPos(int color, int position) {
    // Check that the position in the board is open
    return (board[position] == EMPTY);
}
bool Solver::validRow(int color, int position) {
    // If the value of rowState is negative, there are black queens
    // If the value of rowState is positive, there are white queens

    int row = getRowIndex(position);

    // Logic is like this to match other functions,
    // but inverted to match name of variable
    rowValid = (color == BLACK && rowState[row] <  1) ||
                 (color == WHITE && rowState[row] > -1);
    return rowValid;
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

#if 1
    int fState = f_diagState[fDiag];
    if ( (color == BLACK && fState >  0) ||
         (color == WHITE && fState <  0) )
        return false;

    int rState = r_diagState[rDiag];
    return ( (color == BLACK && rState <  1) ||
             (color == WHITE && rState > -1) );
#else
    return ((color == BLACK && f_diagState[fDiag] <  1) ||
            (color == WHITE && f_diagState[fDiag] > -1)   ) &&
           ((color == BLACK && r_diagState[rDiag] <  1) ||
            (color == WHITE && r_diagState[rDiag] > -1)   );

#endif
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

int Solver::getMaxQueensPlaced() {
    // Find maximum number of queens saved
    //std::cout << "*" << maxQueensPlaced << "*\n";
    std::filesystem::path directoryToCheck = baseOutPath + "/" + std::to_string(maxQueensPlaced++);
    while (!std::filesystem::is_empty( directoryToCheck )) {
        //std::cout << "Looping for max queens... " << maxQueensPlaced << "\n";
        directoryToCheck = baseOutPath + "/" + std::to_string(maxQueensPlaced++);
    }
    maxQueensPlaced -= 2;

    //std::cout << "Max queens placed: " << maxQueensPlaced << "\n";
    return maxQueensPlaced;
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

void Solver::convertStringToState(std::fstream& config_file) {
    // This function reads in a string to update values
    // Get side length
    config_file >> sideLength;

    // Get current queen count
    int queenCount;
    config_file >> queenCount;

    int position;
    // Read in queen positions
    for (size_t i = 0; i < queenCount*2; ++i) {
        config_file >> position;
        addQueen(position);
    }

    // Set current position
    currentIndex = queenPositions[WHITE].back()+1;
    startIndex = queenPositions[WHITE][0];
    startingQueenCount = queenCount;
}
