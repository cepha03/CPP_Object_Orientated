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

    //function for board object. variables to be only used by initBoard
    void initBoard(){

        //assign board components 
        board.assign(size, vector<char>(size, '.'));
        revealed.assign(size, vector<bool>(size, false));
        bombLoc.clear();

        //bombs are to be randomly scattered and not fall in the same block
        srand(time(0));//random number generator. ensures bombs are not in the same block every game
        int bombsPlaced = 0;
        while (bombsPlaced < numBombs) { //loop to place all 10 bombs
            int r = rand() % size; //set coordinates for each bomb. rand() generates a large number. 
            int c = rand() % size; //modulus size of board (remainder)
            if (bombLoc.find({r, c}) == bombLoc.end()) { //list bombloc 
                bombLoc.insert({r, c}); //bomb inserted in correct coordinate
                bombsPlaced++;
            }
        }
        //time set to 0 when game starts. 
        startTime = time(0);
        flips = 0; //flips set to 0 (loss in gravity)
        cout << "Game has started! Bombs hidden: " << numBombs << endl;
    }

    //count mines surrounding each cell when selected
    int countAdjMines(int r, int c) {
        int count = 0; //count set to 0
        for (int dr = -1; dr <= 1; ++dr) { //loop from r-1 to r+1
            for (int dc = -1; dc <= 1; ++dc) { //loop from c-1 to c+1
                if (dr == 0 && dc == 0) continue;
                int nr = r + dr;
                int nc = c + dc;
                //check bounds 
                if (nr >= 0 && nr < size && nc >= 0 && nc < size) {
                    if (bombLoc.count({nr, nc})) { //check if a bomb is at the neighbour location
                        count++;
                    }
                }
            }
        }
        return count; //return total 0 to 8
    }
};
