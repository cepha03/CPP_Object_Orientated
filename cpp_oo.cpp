#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm> 
#include <ctime>     
#include <sstream>  

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

    //recursion flood fill for board fill when a safe neighbour is found
    void floodFill(int r, int c) {
        // Base checks
        if (r < 0 || r >= size || c < 0 || c >= size) 
            return;
        if (revealed[r][c]) 
            return; //already opened
        if (board[r][c] == 'F') 
            return; //don't reveal flags automatically

        revealed[r][c] = true; //mark as opened

        //check surrounding cells for mines
        int mines = countAdjMines(r, c);

        if (mines > 0) {
            //number found, show it and stop
            board[r][c] = mines + '0'; // Convert int to char
        } 
        else 
        {
            //continue recursion if empty. open all 8 neighbours
            board[r][c] = ' '; // Empty space
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    floodFill(r + dr, c + dc); //function called.
                }
            }
        }
    }

    //function to perform gravity flip after 30 seconds have elapsed.
    void performGravityFlip() {
        cout << "\n*** " << (flips + 1) * 30 << " SECONDS.. GRAVITY LOSS! ***\n";
        
        //flip board to simulate loss in gravity
        std::reverse(board.begin(), board.end()); //reverse rows
        for (auto& row : board) {
            std::reverse(row.begin(), row.end()); //reverse row content
        }

        //flip revealed state
        std::reverse(revealed.begin(), revealed.end());
        for (auto& row : revealed) {
            std::reverse(row.begin(), row.end());
        }

        //flip hidden bomb coordinates
        //formula for 180 degree rotation: New_Coord = (Max_Index - Old_Coord)
        set<pair<int, int>> newBombLocs;
        for (auto pos : bombLoc) {
            int newR = (size - 1) - pos.first;
            int newC = (size - 1) - pos.second;
            newBombLocs.insert({newR, newC});
        }
        bombLoc = newBombLocs; //replace old bomb list with new, once rotation occurs

        flips++;
    }

    //function to display board and its components. 
    //time and flips are displayed
    void displayBoard(double elapsed) {
        cout << "\n--- Time: " << (int)elapsed << "s | Flips: " << flips << " ---" << endl; 
        cout << "  "; //print column headers
        for(int i=0; i<size; ++i) cout << i << " ";
        cout << endl;
        for (int i = 0; i < size; ++i) {
            cout << i << " "; //print row column headers
            for (int j = 0; j < size; ++j) {
                cout << board[i][j] << " "; //display cells
            }
            cout << endl;
        }
    }

public: 
    //constructor to run automatically when a new game object is created
    MinesweeperOO(int s = 9, int nb = 10) //initial size and bomb count set
        : size(s), numBombs(nb), gameOver(false) { //encapsulation of data
        initBoard(); //board layout function called
    }

    //function to hold all game attributes
    void playGame() {
        while (!gameOver) {
            time_t currentTime = time(0); //time check
            double elapsed = difftime(currentTime, startTime); //calculates time passed

            // PERIODIC FLIP CHECK
            // Checks if we crossed a 30s threshold (30, 60, 90...)
            if (elapsed > (flips + 1) * 30) {
                performGravityFlip();
            }

            displayBoard(elapsed);

            //input handling
            cout << "\nCommands: Enter 'r c' or 'f r c' (flag)" << endl;
            cout << "Enter command: ";
            
            string op;
            int r, c;
            
            if (!(cin >> op)) 
                break;//if input fails exit loop

            //handle flag input
            if (op == "f") {
                cin >> r >> c;
                //toggle flag 
                if (r >= 0 && r < size && c >= 0 && c < size) {
                    if (!revealed[r][c]) {
                        if (board[r][c] == '.') board[r][c] = 'F';
                        else if (board[r][c] == 'F') board[r][c] = '.';
                    }
                }
            }
            //handle user entering number to play
            else {
                // If op wasn't 'f', it must be the row number
                try {
                    r = stoi(op); //convert string to int
                    cin >> c; //get column number
                    
                    //error handling, if f is selected but flag present.. then show message
                    if (r >= 0 && r < size && c >= 0 && c < size) {
                        if (board[r][c] == 'F') {
                            cout << "Cannot select a flag! Unflag it first." << endl;
                        } else if (bombLoc.count({r, c})) {
                            board[r][c] = 'B'; // Show bomb
                            displayBoard(elapsed);
                            cout << "\nBOOM! You triggered a mine. GAME OVER." << endl;
                            gameOver = true;
                        } else {
                            floodFill(r, c); // Start Flood Fill
                            
                            // Check Win Condition
                            int coveredCount = 0;
                            for(int i=0; i<size; i++) 
                                for(int j=0; j<size; j++) 
                                    if(!revealed[i][j]) coveredCount++;
                            
                            if(coveredCount == numBombs) {
                                displayBoard(elapsed);
                                cout << "\nVICTORY! All mines cleared." << endl;
                                gameOver = true;
                            }
                        }
                    } else {
                        cout << "Invalid coordinates." << endl;
                    }
                } catch (...) {
                    cout << "Invalid input." << endl;
                    cin.clear(); // Clear error flags
                    string ignoreLine; getline(cin, ignoreLine); // Skip bad line
                }
            }
        }
    }
};
