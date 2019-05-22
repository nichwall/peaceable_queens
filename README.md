# Peaceable Queens
Attempts to solve the Peaceable Queens problem, as described in this video https://www.youtube.com/watch?v=IN1fPtY9jYg

This solution is designed to be single threaded, so that it can be implemented on an FPGA using digital logic. Multiple threads would probably speed it up, but not a lot of thought has been put into that application beyond having each thread have the starting queen be at different locations such that they don't run over each other in the backtracking algorithm. All benchmarks demonstrating changes in time are generalizations from running the code, without in-depth analysis.

## General algorithm

To solve this problem, two vectors are created, one for the positions of white queens and one for the positions of black queens. Whenever there is an equal number of black and white queens placed (0, 1, 2, etc), it is White's turn. When they are unequal, it is Black's turn. The index of the board to begin searching at is the last position of the last queen placed plus one. If the white queens had positions of `{0,1,3,6}`, the index to begin searching would be `7`. If the position results in a legal board, then this action is repeated for the other color until there is an illegal placement. If the newly placed piece results in an invalid board, then the piece is removed, and the search continues until the end of the board is reached. If the end of the board is reached without a valid position being found, then the previous queen placed is removed, the index is noted, and the search continues from there.

## Beneficial optimizations

If the point of invalidity is the row, skip to the first index of the next row. This cut the time by about 40% on the smaller boards, but the larger boards still take long enough to not be tested without this optimization.

Only allow the first white queen to be in the top left corner of the board. This prevents any rotations, vertical reflections, and horizontal reflections from being found, as well as reduces the search space.

Only allow the first black queen to be above the digaonal of the board that is from the top left to the bottom right. This prevents any reflections along the diagonal from being found, as well as reduces the search space.

No black queens can be located on the diagonal from the upper left corner to the lower right corner. Any black queens located on this diagonal will result in the inversion of having a white queen in that location.

When checking if a board configuration is valid, compare the number of remaining spaces to the end of the board to the best solution found to this point. For example, if at a point during the search we have found the most queens successfully placed to be 7, and the second queen is attempting to be placed within 6 locations of the end of the board, we know that all resulting boards from that configuration will not be optimal, so skip them and backtrack. This resulted in a speedup of around 30-40% on boards 7x7 and smaller.

## Detrimental optimizations

Keeping track of all previous solutions in order to check for permutations of the same board. This led to an increase in time to solve all board configurations, increase in memory usage, and added a lot of complexity to make sure each perumation was found. Most permutations are found from restricting the location of the starting queens, found above. Attempts were made to convert the vector to a string before hashing, as well as preallocating memory, but no beneficial variation was found.

Tracking columns for skipping undesired indices when searching for the next valid location. Tracking these through the use of flags results in more comparisons per board configuration, and cleaning up the flags after a queen is successfully placed or removed would also increase the amonut of work per board configuration. Code was not implemented, but determined to be ineffective when not using digital logic.

## Untested optimizations

## Optimizations for the FPGA

Unknown which of the detrimental optimizations to keep, additional planning required.
