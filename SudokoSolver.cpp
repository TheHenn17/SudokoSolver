//This program will let the user input a sudoko board, 9 rows of 9 numbers (0 for blank spaces),
//and it will complete the board using normal sudoko rules
//This program uses a limited depth search algorithm in order to complete the board,
//and so it must hold potential board states and their depth in the search tree
#include <iostream>
#include <stack>
using namespace std;
//test
//node struct holds a potential sudoko board and its depth in the search tree
struct node {
    int** state;
    int depth;
};

//gathers sudoko board input from the user and stores it in a 2D array
int** createPuzzle();

//solves the input puzzle using limited depth search with a limit of 81 (hardcoded)
void LDS(int** puzzle);

//returns a new instance of the given 2D array
int** cpy(int** puzzle);

//Tests if the given sudoko puzzle is solved (does not break any sudoko rules)
bool GoalTest(int** state);

//Expands the search tree with the children of a given sudoko board, adds valid children to the list
void Expand(stack<node*>& list, node* parent);

//gets the puzzles depth in the search tree by counting how many nonzero numbers are on the board
int getDepth(int** puzzle);

//checks if the number given by row and col in the given puzzle breaks any sudoko rules
bool isValid(int** puzzle, int row, int col);

//displays a given puzzle to the console
void outputPuzzle(int** state);

//deletes the given puzzle from the heap
void deletePuzzle(int** puzzle);

//size of one side of the sudoko board
const int SIZE = 9;

int main()
{
    //declare a 2D array
    int** puzzle = 0;

    cout << "Welcome to Thomas Henningson's Suduko Solver.\n\n";

    //Instantiate 2D array with user input
    puzzle = createPuzzle();

    //solves input puzzle and outputs solution, if any
    LDS(puzzle);

    //deletes array for cleanup
    deletePuzzle(puzzle);

    return 0;
}

//gathers sudoko board input from the user and stores it in a 2D array
int** createPuzzle() {
    //declare new 2D array in the heap
    int** puzzle = new int* [SIZE];
    for (int i = 0; i < SIZE; i++) {
        puzzle[i] = new int[SIZE];
    }
    cout << "Enter your puzzle, use a zero to represent the blanks\n\n";

    //Populate array with user input
    for (int i = 0; i < SIZE; i++) {
        cout << "Enter row " << i + 1 << ", use space or tabs between numbers   ";
        for (int j = 0; j < SIZE; j++) {
            cin >> puzzle[i][j];
        }
    }
    return puzzle;
}

//solves the input puzzle using limited depth search with a limit of 81 (hardcoded)
void LDS(int** puzzle) {
    //declare a stack and a limit in order to use depth limited search
    stack<node*> list;
    int limit = 81;

    //instatiate the first node in the stack with a copy of the users puzzle
    node* n = new node;
    n->state = cpy(puzzle);
    n->depth = getDepth(puzzle);
    list.push(n);

    //Begin the LDS algorithm
    while (!list.empty()) {
        //pop element off of stack (LIFO ensures depth first search in the search tree)
        n = list.top();
        list.pop();

        //If the depth of the popped node is at the limit depth, all members of the 2D array have a value 1-9
        if (n->depth == limit) {
            //Therefore, we check if this particular puzzle breaks any rules of sudoko
            if (GoalTest(n->state)) {
                //If it does not, this puzzle is solved, so we output the solution and delete the rest of the search tree, and return
                cout << endl;
                outputPuzzle(n->state);
                deletePuzzle(n->state);
                while (!list.empty()) {
                    n = list.top();
                    list.pop();
                    deletePuzzle(n->state);
                }
                return;
            }
        }
        //puzzle is not at the solution depth, so we must expand the tree to the next depth
        else {
            Expand(list, n);
        }

        //delete this puzzle once we are done with it
        deletePuzzle(n->state);
    }

    //outside the loop and the algorithm has found no solution, meaning there is not one
    cout << "No Solution.\n";
}

//returns a new instance of the given 2D array
int** cpy(int** puzzle) {
    int** cpy = new int* [SIZE];
    for (int i = 0; i < SIZE; i++) {
        cpy[i] = new int[SIZE];
    }
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cpy[i][j] = puzzle[i][j];
        }
    }
    return cpy;
}

//Tests if the given sudoko puzzle is solved (does not break any sudoko rules)
bool GoalTest(int** state) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            //If one value of the puzzle is not valid, then the puzzle is a not solution
            if (!isValid(state, i, j)) {
                return false;
            }
        }
    }
    //if every value is valid, this puzzle is a solution
    return true;
}

//Expands the search tree with the children of a given sudoko board, adds valid children to the list
void Expand(stack<node*>& list, node* parent) {
    int row = -1;
    int col = -1;
    int** childState;
    node* child;
    //find the row and column of the next empty value of the puzzle
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (parent->state[i][j] == 0) {
                row = i;
                col = j;
                break;
            }
        }
    }
    if (row < 0 || col < 0) {
        //for each possible value of the empty square
        for (int i = 1; i <= 9; i++) {
            //create a copy of the board but change the empty value
            childState = cpy(parent->state);
            childState[row][col] = i;
            //if this new value is valid, the child is valid and is added to the list
            if (isValid(childState, row, col)) {
                child = new node;
                child->state = childState;
                child->depth = parent->depth + 1;
                list.push(child);
            }
            //if the new value is not valid, then we delete this particular child and dont add it to the list
            else {
                deletePuzzle(childState);
            }
        }
    }
}

//gets the puzzles depth in the search tree by counting how many nonzero numbers are on the board
int getDepth(int** puzzle) {
    int depth = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (puzzle[i][j] != 0) {
                depth++;
            }
        }
    }
    return depth;
}

//checks if the number given by row and col in the given puzzle breaks any sudoko rules
bool isValid(int** puzzle, int row, int col) {
    for (int i = 0; i < SIZE; i++) {
        //if any values in the same row have the same as the given, it is not valid
        if (puzzle[row][i] == puzzle[row][col] && col != i) {
            return false;
        }
        //if any values in the same column have the same as the given, it is not valid
        if (puzzle[i][col] == puzzle[row][col] && row != i) {
            return false;
        }
    }
    //The following if statements cover the 9 squares in the sudoko board which also must have different values
    if (row <= 2) {
        if (col <= 2) {
            //value we are checking is in the top left 9th square
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (puzzle[i][j] == puzzle[row][col] && row != i && col != j) {
                        return false;
                    }
                }
            }
        }
        else if (col >= 6) {
            //value we are checking is in the top right 9th square
            for (int i = 0; i < 3; i++) {
                for (int j = 6; j < 9; j++) {
                    if (puzzle[i][j] == puzzle[row][col] && row != i && col != j) {
                        return false;
                    }
                }
            }
        }
        else {
            //value we are checking is in the top middle 9th square
            for (int i = 0; i < 3; i++) {
                for (int j = 3; j < 6; j++) {
                    if (puzzle[i][j] == puzzle[row][col] && row != i && col != j) {
                        return false;
                    }
                }
            }
        }
    }
    else if (row >= 6) {
        //value we are checking is in the bottom left 9th square
        if (col <= 2) {
            for (int i = 6; i < 9; i++) {
                for (int j = 0; j < 3; j++) {
                    if (puzzle[i][j] == puzzle[row][col] && row != i && col != j) {
                        return false;
                    }
                }
            }
        }
        else if (col >= 6) {
            //value we are checking is in the bottom right 9th square
            for (int i = 6; i < 9; i++) {
                for (int j = 6; j < 9; j++) {
                    if (puzzle[i][j] == puzzle[row][col] && row != i && col != j) {
                        return false;
                    }
                }
            }
        }
        else {
            //value we are checking is in the bottom middle 9th square
            for (int i = 6; i < 9; i++) {
                for (int j = 3; j < 6; j++) {
                    if (puzzle[i][j] == puzzle[row][col] && row != i && col != j) {
                        return false;
                    }
                }
            }
        }
    }
    else {
        if (col <= 2) {
            //value we are checking is in the middle left 9th square
            for (int i = 3; i < 6; i++) {
                for (int j = 0; j < 3; j++) {
                    if (puzzle[i][j] == puzzle[row][col] && row != i && col != j) {
                        return false;
                    }
                }
            }
        }
        else if (col >= 6) {
            //value we are checking is in the middle right 9th square
            for (int i = 3; i < 6; i++) {
                for (int j = 6; j < 9; j++) {
                    if (puzzle[i][j] == puzzle[row][col] && row != i && col != j) {
                        return false;
                    }
                }
            }
        }
        else {
            //value we are checking is in the true middle 9th square
            for (int i = 3; i < 6; i++) {
                for (int j = 3; j < 6; j++) {
                    if (puzzle[i][j] == puzzle[row][col] && row != i && col != j) {
                        return false;
                    }
                }
            }
        }
    }
    //if all checks pass, the value at row and col is valid
    return true;
}

//displays a given puzzle to the console
void outputPuzzle(int** state) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cout << state[i][j] << " ";
        }
        cout << endl;
    }
}

//deletes the given puzzle from the heap
void deletePuzzle(int** puzzle) {
    for (int i = 0; i < SIZE; i++) {
        delete[] puzzle[i];
    }
    delete[] puzzle;
}
