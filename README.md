# Peaceable Queens
Attempts to solve the Peaceable Queens problem, as described in this video https://www.youtube.com/watch?v=IN1fPtY9jYg

## General algorithm

To solve this problem, two vectors are created, one for the positions of white queens and one for the positions of black queens. Whenever there is an equal number of black and white queens placed (0, 1, 2, etc), it is White's turn. When they are unequal, it is Black's turn. The index of the board to begin searching at is the last position of the last queen placed plus one. If the white queens had positions of `{0,1,3,6}`, the index to begin searching would be `7`. If the position results in a legal board, then this action is repeated for the other color until there is an illegal placement. If the newly placed piece results in an invalid board, then the piece is removed, and the search continues until the end of the board is reached. If the end of the board is reached without a valid position being found, then the previous queen placed is removed, the index is noted, and the search continues from there.

## Beneficial optimizations

If the point of invalidity is the row, skip to the first index of the next row. This cut the time by about 40% on the smaller boards, but the larger boards still take long enough to not be tested without this optimization.

Only allow the first white queen to be in the top left corner of the board. This prevents any rotations, vertical reflections, and horizontal reflections from being found, as well as reduces the search space.

Only allow the first black queen to be on or above the digaonal of the board. This prevents any reflections along the diagonal from being found, as well as reduces the search space.

## Detrimental optimizations

Keeping track of all previous solutions in order to check for permutations of the same board. This led to an increase in time to solve all board configurations, increase in memory usage, and added a lot of complexity to make sure each perumation was found. Most permutations are found from restricting the location of the starting queens, found above. Attempts were made to convert the vector to a string before hashing, as well as preallocating memory, but no beneficial variation was found.

Tracking columns for skipping undesired indices when searching for the next valid location. Tracking these through the use of flags results in more comparisons per board configuration, and cleaning up the flags after a queen is successfully placed or removed would also increase the amonut of work per board configuration.

## Untested optimizations

- Check if there's enough space left in the board to match the best solution found up to that point
- Remove the position occupied check, because that is taken care of in row, column, and diagonal checking.

## Optimizations for the FPGA

Unknown which of the detrimental optimizations to keep, additional planning required.
