#ifndef CONNECTN_HPP
#define CONNECTN_HPP
//guards to avoid unexpected bugs and glitches

#include <iostream>
#include <iomanip>
#include <fstream> //to read and write in text files
#include <ctime> //to time the game
#include <string>
#include <vector>
#include <algorithm> //for sort function
#include <windows.h> //for sleep function
#include <cstdlib> 

using namespace std;

// CLASSES:
class Board
{
    public:

        int n; //either 4 or 5 for our code. it is the amount in a line needed to win and changes grid size
        int grid[7][8]; //grid initialized to max value possible. if smaller needed then less area used  
        int filled[8]; //used to record the next available row to put disc in. -1 if column filled.
        int total_rows,total_columns; 

        Board(int number); //takes in the number related to the game. 4 or 5
        void display(); //displays board nicely
        int addToColumn(int column, int player); //adds the player number into the column specified
        int winCondition(int column, int player); //checks if the specified player has won for column. used directly after addToColumn and checks all possible wins
        bool drawCondition(); //checks if the game has ended in a draw
        void calculateFilledState(); //populates the filled array in the loadGame function
};

class Player //the parameters and class in general is mainly to make it easier to save and load data into leaderboard and save file.
{
    public:
        string name;
        int identifier;
        int turns;
        int score;
        int time;

        Player(string player_name, int player_num, int num_turns, int total_score, int time_elapsed); //initializes all attributes to the values passed
        void print(); //outputs the name and score formatted well. used to output leaderboard
        bool operator>(const Player& other) const; //comparison operator for sorting
};

class Leaderboard
{
    private:
        string file; //variable for each object made
        string filenames[2][2] = {{"leaderboard_1P_4.txt","leaderboard_1P_5.txt"},{"leaderboard_2P_4.txt","leaderboard_2P_5.txt"}}; //possible values for file
    
    public:
        vector<Player> players; //stores records from text file in vector of type Player

        Leaderboard(int boardsize, int players); //takes the board size (4 or 5) and the number of players (1 or 2) and sets value of file
        int makeVector(); //reads file and populates the vector
        void displayLeaderboard(int n); //Displays top n records of the leaderboard(will give n=15 if want top 15 or a negative number if I want all records)
        void updateLeaderboard(Player winner); //adds the winner into the vector
        void saveLeaderboard(); //saves updated vector into the respective leaderboard file. called right after updateLeaderboard
};

//FUNCTIONS
void clearBadInput(); //clears cin flags and ignores incorrect input like a letter input instead of integer etc. It is repeated often
int showMenu(); //displays menu and takes in the choice of the user. Validates the input as well
void saveGame(Player players[], Board board); //saves the game with these players and this board into the text file. 
void playGame2P(Board &board); //plays a 2 player game with this board
void playGame1P(Board &board); //plays a 1 player game with this board
int resumeGame(Player &player1, Player &player2, Board &board); //used inside loadGame to actually play the game
void loadGame(); //reads all games from save file and displays them. asks user which game to play and calls resumeGame to play that. Removes that game from the save file.

#endif 