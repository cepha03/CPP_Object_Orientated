#include <iostream>
#include <string>
#include <vector>
#include <set>

using namespace std;

//one private class with all relavent variables. Private is used to hide the internal state.
//this is required to hold the OOP attributes
class MinesweeperOO {
private:

    int size; //size of the board
    int numBombs; //num of bombs
    vector<vector<char>> board; //the visual board in the console
    vector<vector<bool>> revealed;//tracks if a cell is open
    set<pair<int, int>> bombLoc;//stores coordinates of bombs
    time_t startTime;
    int flips;//tracks how many times we've flipped
    bool gameOver;
};
