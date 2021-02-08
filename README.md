# SudokoSolver
## Description
This is a basic Sudoko Solver. It uses normal sudoko rules\* to take a user generated input puzzle, find a solution, and output it. This program does not allow for Sudoko puzzles of sizes other than the typical 9x9 to be solved, and it does not allow for other rules to be taken into account.

\*normal sudoko rules mean that each row has a complete set of 1-9 numbers. Each column also has a complete set of 1-9 numbers. Finally, the 9x9 is also divided into 9 equal 3x3 squares, which also must have a complete set of 1-9 numbers.

## Execution
After a welcome message, the user is asked to input a sudoko board row by row, using 0 as a representation of the blank space. The user should put a space or tab between the values to distinguish them. Upon completion of the 9th row, the algorithm begins running and will complete in a few seconds. The program then outputs the solution (or no solution) to the console and exits

## Implementation
This program uses a version of Depth First search known as Depth-Limited Search. The idea is that if you know what depth in the search tree\* your problem solution is, you can race down the tree to potential solutions and check them without bothering to check nodes before the solution depth. Since any solution must have all squares of the sudoko board filled out, we know the "depth" of the solution must be 9x9 = 81. 

So, once the user has finished inputting the puzzle, we simply count how many non-empty squares they have given, and that becomes our starting depth. From there, we simply perform Depth First Search, filling in each square with a potential value until we come to depth of 81, a potential solution. We check if this solution is indeed a valid solution, and if it is we have completed the algorithm. Otherwise, we backtrack up the tree to a lower depth and fill in squares until 81 is reached again.

In the case that that all possible puzzles at 81 using the given numbers are invalid, the loop breaks and no solution is outputted.

\* A search tree is a representaion that can be used to solve puzzles. Basically, the initial state of the puzzle is the root of your tree, and the children of each node are reached though defined operators, and each node is tested to see if it solves the puzzle. In this case, our operators are that one number 1-9 can be added to an empty square at each level. So, at most each node has 9 children, but most likely less since any number added to the blank spot is checked for sudoko validity.
