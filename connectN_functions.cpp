// function to run the game in terminal is: g++ connectN_main.cpp connectN_functions.cpp -o connectNgame; .\connectNgame

#include "connectN.hpp" //include header file
/*Check the header file for general overview of what each function does*/

//CLASS CONSTRUCTORS AND FUNTIONS:
Board::Board(int number)
{
    n = number;     //number is either 4(6x7 grid) or 5(7x8 grid)
    total_rows = n+2;
    total_columns = n+3;

    for(int i=0;i<total_rows;i++)     //initializes all cells to 0
    {
        for(int j=0;j<total_columns;j++){
            grid[i][j]=0;
        }
    }

    for(int i=0;i<8;i++) //next row is 0 at the start
        filled[i]=0;

    
}

void Board::display()
{
    cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"; //LARGE space
    
    //display column numbers
    cout << "   ";
    for (int i=1;i<=total_columns;i++)
        cout << i << "     ";
    cout << endl;
    
    //display grid with borders
    for(int x=0;x<total_columns;x++)
        cout << "+-----";
    cout << '+' << endl;

    for(int i=total_rows-1;i>=0;i--) //for rows: displays last row first and first row at the bottom as that is how game logic operates. rows filled bottom to top---> first to last
    {
        cout << '|'; //left border
        for(int j=0;j<total_columns;j++){
            cout << "  ";
            
            if (grid[i][j]==0) //to make an empty cell appear blank
                cout << " ";
            else if(grid[i][j]==1) //Player 1=X
                cout << "X";
            else
                cout << "O"; //Player 2 = O
            
                cout << "  |"; //right border
        }
        cout << endl;
        for(int x=0;x<total_columns;x++)
            cout << "+-----"; //bottom border for each row
        cout << '+' << endl;
    }
    cout << "Player 1 = X\t\tPlayer 2 = O\n"; //Info for the user

}

int Board::addToColumn(int column, int player)
{
    column--; //for c++ indexing

    if (filled[column]==-1) //if column full return 0 as adding was unsuccessful
        return 0; 


    grid[filled[column]][column] = player; //put player into specific spot and increment filled array element
    filled[column]++;

    if (filled[column]==total_rows) //if column now full, put -1 in filled array element
        filled[column] = -1;
    return 1;
}

int Board::winCondition(int column, int player)
{
    column--;
    int row = filled[column]-1;

    //check the row
    int consecutive_matches = 0;
    for(int y=0;y<total_columns;y++){
        if(grid[row][y]==player)
            consecutive_matches++;
        else
            consecutive_matches = 0;

        if(consecutive_matches==n)
            return 1;
    }

    //check for column
    consecutive_matches = 0;
    for(int x=0;x<total_rows;x++){
        if(grid[x][column]==player)
            consecutive_matches++;
        else
            consecutive_matches = 0;

        if(consecutive_matches==n)
            return 1;
    }

    int curr_row, curr_col;
    consecutive_matches=0;
    //check for forward diagonal
    for(int change=-7;change<=7;change++) //some unnecesary loops may run but easier than going through all calculations of how much to check in each direction
    {
        curr_row = row+change;
        curr_col = column+change;

        if (curr_col<0||curr_col>=total_columns||curr_row<0||curr_row>=total_rows) //check for invalid
            continue;
        
        if (grid[curr_row][curr_col]==player)
            consecutive_matches++;
        else
            consecutive_matches = 0;
        if(consecutive_matches==n)
            return 1;
    }

    consecutive_matches=0;
    //check for backward diagonal
    for(int change=-7;change<=7;change++) //some unnecesary loops may run but easier than going through all calculations of how much to check in each direction
    {
        curr_row = row+change;
        curr_col = column-change; //only change needed in code from other diagonal. If going back in row, go up in column and vice versa.

        if (curr_col<0||curr_col>=total_columns||curr_row<0||curr_row>=total_rows) //check for invalid
            continue;
        
        if (grid[curr_row][curr_col]==player)
            consecutive_matches++;
        else
            consecutive_matches = 0;
        if(consecutive_matches==n)
            return 1;
    }

    return 0; //if no win condition satisfied
}

bool Board::drawCondition()
{
    bool draw = true;

    for (int i=0;i<total_columns;i++)
    {
        if(filled[i]!=-1)
            draw=false;
    }
    return draw;
    
}

void Board::calculateFilledState()
{
    for (int j = 0; j < total_columns; j++)
    {
        int current_row = 0;
        while (current_row < total_rows && grid[current_row][j] != 0) //run loop till in range and cell is empty
            current_row++;
        
        filled[j] = (current_row == total_rows) ? -1 : current_row;
    }
}


Player::Player(string player_name, int player_num, int num_turns, int total_score, int time_elapsed)
{
    name = player_name;
    identifier = player_num;
    turns = num_turns;
    score = total_score;
    time = time_elapsed;
}

void Player::print()
{
    cout << left <<  setw(30) << name;
    cout << score << endl;
}

bool Player::operator>(const Player& other) const {
    return score > other.score;
}


Leaderboard::Leaderboard(int boardsize, int players)
{
    file = filenames[players-1][boardsize-4]; //correct mapping of inputs onto the file needed
}

int Leaderboard::makeVector()
{
    string name,line;
    int score, runs;
    

    fstream leaderboard(file,ios::in); //open file for read

    if(!leaderboard.is_open()) //if file doesn't exist or did not open properly return 0
    {
        return 0;
    }

    while(leaderboard >> name >> score) //run loop while you are getting these values.
    {
        players.push_back(Player(name,1,0,score,0)); //put all elements in text file into a vector
    }

    leaderboard.close(); //close file
    return 1; //successful
}

void Leaderboard::displayLeaderboard(int n)
{
    if (players.empty())
    {
        cout << "No records found!\n";
        return; //if file exists but is empty, exit
    }

    int count =1; //keeps track of list
    sort(players.begin(),players.end(),greater<Player>()); //sort the vector players in descending order according to the operator defined in it

    cout << "======================================\n               LEADERBOARD\n======================================\n";
    for(Player element : players) //print all elements in the sorted vector. 
    {
        if (n==0) //to end the loop after 20 records. (if applicable) 
            break;
        cout << right << setw(2) << setfill('0') << count++ << ". "; //formatted number. print 01 not 1
        cout << setfill(' ');
        element.print();
        n--;
    }
}

void Leaderboard::updateLeaderboard(Player winner)
{
    bool updated = false, found = false;

    for(int i=0;i<winner.name.length();i++)
    {
        if(winner.name[i]==' ')
            winner.name[i]='_';
    } // replace space in name with underscore so that it reads at once in text file

    for (Player &element : players) //byref passing so can change
    {
        if(element.name==winner.name)
        {
            found = true;
            if (element.score<winner.score){
                element.score = winner.score;
                cout << "Old record found and updated\nNew score: " << winner.score << endl;
                updated=true;}
            //if pre existing record for the player and the new score is higher than previous then update it
        }
    }

    if(!found)
    {
        players.push_back(winner); //if new record add it to the vector
        cout << "Record added to leaderboard\n";
    }
    else
        if(!updated)
            cout << "Your current score is not higher than the past score you achieved\n";
    
}

void Leaderboard::saveLeaderboard()
{
    ofstream leaderboard(file);

    for (Player element : players)
    {
        leaderboard << element.name << " " << element.score << '\n';
    }

    leaderboard.close();
}



//FUNCTIONS:
void clearBadInput()
{
    if(cin.fail()){
        cin.clear();
        cin.ignore(10000,'\n');
    }
    else
        cin.ignore(10000,'\n');
}

int showMenu()
{
    int choice;

    cout << "\n================================================\n                       MENU\n================================================\n";
    cout << "1. How To Play ConnectN\n2. New Game (6x7 Grid, Connect 4)\n3. New Game (7x8 Grid, Connect 5)\n4. Load Game\n5. View Leaderboards\n6. Exit Program\n";
    cout << "================================================\n";
    cout << "Enter your choice: ";
    cin >> choice;
    clearBadInput();
    while(choice<1 || choice>6){
        cout << "Not a valid option. Enter Again.\n";
        choice = showMenu(); //if invalid input, ask again
    }
    
    return choice;
}

void saveGame(Player players[], Board board)
{
    fstream savefile("gamesaves.txt",ios::app); //open file to append

    for(int x=0;x<2;x++)
    {
        for(int i=0;i<players[x].name.length();i++)
            {
                if(players[x].name[i]==' ')
                    players[x].name[i]='_';
            } // replace space in names with underscore so that it reads at once in text file
    }

    savefile << board.n << " " << players[0].name << " " << players[1].name << " " << players[0].turns << " " << players[0].time;
    //add game attributes into the text file in the decided order
    
    for(int i = 0; i<board.total_rows;i++)
    {
        for(int j=0; j<board.total_columns; j++)
        {
            savefile << " " << board.grid[i][j];
        }
    }
    savefile << '\n';
    //put game grid into the text file

    savefile.close();
}

void playGame2P(Board &board)
{
    string player1name,player2name;

    //get player names
    cout << "Player 1, Enter your name: ";
    getline(cin,player1name);
    Player player1(player1name,1,0,0,0);
    cout << "Player 2, Enter your name: ";
    getline(cin,player2name);
    Player player2(player2name,2,0,0,0);

    Player players[2] = {player1,player2};

    int currplayer = 1;
    int user_input;
    bool endgame=false;


    time_t start = time(0); //get start time

    do{

        board.display();
        cout << players[currplayer-1].name << " enter column to put disc in (or enter 83 to save the game and return to menu): ";
        cin >> user_input;
        clearBadInput();

        if (user_input==83) //save (ASCII for S is 83)
        {
            time_t end = time(0);
            double seconds = difftime(end,start);
            players[0].time = seconds;
            saveGame(players,board);
            break;
        }
        while((user_input>board.total_columns || user_input<1) && user_input!=83){
            cout << "Invalid column number!\n";
            cout << players[currplayer-1].name << " enter column to put disc in (or enter 83 to save the game and return to menu): ";
            cin >> user_input;
            clearBadInput();
        }

        int added = board.addToColumn(user_input,currplayer);
        while(added==0){ //repeat if column is full
            cout << "This column is full.\n";
            cout << players[currplayer-1].name << " enter column to put disc in: ";
            cin >> user_input;
            clearBadInput();
            added = board.addToColumn(user_input,currplayer);
        }
        players[0].turns++;
        players[1].turns++;
        
        int won = board.winCondition(user_input,currplayer);
        if(won)
        {
            board.display(); //display winning board
            cout << '\n' << players[currplayer-1].name << " won the game! Congratulations!!\n";
            time_t end = time(0);
            double seconds = difftime(end,start);
            cout << "That game took " << seconds << " seconds\n";
            players[currplayer-1].time = seconds;
            players[currplayer-1].score = 30000/(players[currplayer-1].time+5*players[currplayer-1].turns); //arbritrary score formula we came up with
            cout << "\nYou got a score of " << players[currplayer-1].score << endl;

            //ask to save in leaderboard, and if yes, then save
            int choice;

            do{
            cout << "Would you like to save this record to the leaderboard? (1 for Yes, 0 for No): ";
            cin >> choice;
            clearBadInput();
            }while(choice<0||choice>1);

            cout << "\n";
            if(choice){
                Leaderboard leader(board.n,2);
                int madeleaderboard = leader.makeVector();
                leader.updateLeaderboard(players[currplayer-1]);
                leader.saveLeaderboard();
            }

            endgame=true;
        }
        else
        {
            currplayer = 3-currplayer;
        }

        if(!won && board.drawCondition()) //check for draw at the end of turn if not won
        {
            board.display();
            cout << "\nThe game has ended in a draw! The board is full. Better luck next time\n";
            time_t end = time(0);
            double seconds = difftime(end,start);
            cout << "That game took " << seconds << " seconds\n";
            endgame=true;
        }
        
    }while(!endgame);
}

void playGame1P(Board &board)
{
    string player1name;

    cout << "Player 1, Enter your name: ";
    getline(cin,player1name);
    Player player1(player1name,1,0,0,0);
    Player playerai("The Creators",2,0,0,0);
    Player players[2] = {player1,playerai};

    int currplayer = 1;
    int user_input;
    bool endgame=false;


    time_t start = time(0); //get start time

    do{

        board.display();
        if(currplayer==1)
        {
            if(players[0].turns>0) //output AI choice if this isn't the first turn
                cout <<"The Creators have chosen to put disc in column " << user_input << endl;
            cout << players[currplayer-1].name << " enter column to put disc in (or enter 83 to save the game and return to menu): ";
            cin >> user_input;
            clearBadInput();
        }
        else
        {
            user_input = rand()%board.total_columns+1; //random choice between possible columns
        }
        if (user_input==83)
        {
            time_t end = time(0);
            double seconds = difftime(end,start);
            players[0].time = seconds;
            saveGame(players,board);
            break;
        }
        while((user_input>board.total_columns || user_input<1) && user_input!=83){
            cout << "Invalid column number!\n";
            cout << players[currplayer-1].name << " enter column to put disc in (or enter 83 to save the game and return to menu): ";
            cin >> user_input;
            clearBadInput();
        }

        int added = board.addToColumn(user_input,currplayer);
        while(added==0){
            if(currplayer==1){
                cout << "This column is full.\n";
                cout << players[currplayer-1].name << " enter column to put disc in: ";
                cin >> user_input;
                clearBadInput();
            }
            else
                user_input = rand()%board.total_columns+1;
            added = board.addToColumn(user_input,currplayer);
        }
        
        players[0].turns++;
        players[1].turns++;

        int won = board.winCondition(user_input,currplayer);
        if(won)
        {
            board.display();
            cout << '\n' << players[currplayer-1].name << " won the game! Congratulations!!\n";
            time_t end = time(0);
            double seconds = difftime(end,start);
            cout << "That game took " << seconds << " seconds\n";
            players[currplayer-1].time = seconds;
            

            if(currplayer==1)
            {
                players[currplayer-1].score = 30000/(players[currplayer-1].time+5*players[currplayer-1].turns); //arbritrary score formula we came up with
                cout << "\nYou got a score of " << players[currplayer-1].score << endl;

                //ask to save in leaderboard, and if yes, then save
                int choice;

                do{
                cout << "Would you like to save this record to the leaderboard? (1 for Yes, 0 for No): ";
                cin >> choice;
                clearBadInput();
                }while(choice<0||choice>1);
                
                if(choice){
                    Leaderboard leader(board.n,1);
                    int madeleaderboard = leader.makeVector();
                    leader.updateLeaderboard(players[currplayer-1]);
                    leader.saveLeaderboard();
            }
            }

            endgame=true;
        }
        else
        {
            currplayer = 3-currplayer; //switch players
        }

        if(!won && board.drawCondition()){
            board.display();
            cout << "\nThe game has ended in a draw! The board is full. Better luck next time\n";
            time_t end = time(0);
            double seconds = difftime(end,start);
            cout << "That game took " << seconds << " seconds\n";
            endgame=true;
        }
        
    }while(!endgame);
}


int resumeGame(Player &player1, Player &player2, Board &board)
{
    Player *players[2] = {&player1, &player2}; //had to use a pointer and refernece variables here because previous approach was giving errors. learnt this and did a lot of debugging
    int currplayer,user_input;
    bool endgame=false;
    bool firstturn = true;
    time_t start = time(0);
    bool game_against_ai= (player2.name=="The Creators") ; //only checking player 2 as code only puts randomizer as player 2

    if(player1.turns%2) //if odd then its player 2's turn and vice versa
        currplayer=2;
    else
        currplayer=1;
    
    
    do //run game loop
    {
        board.display();

        if(firstturn) //was personally confused when game would load on who's turn it is
            cout << "It is Player "<<currplayer<<"'s turn\n";
        

        if(players[currplayer-1]->name!="The Creators") //had to learn -> operator to reference properly
        { 
            if(game_against_ai && !firstturn) //show randomizer choice but not if its the first turn
                cout << "The Creators have chosen to put their disc in column "<<user_input<<endl;
        
            cout << players[currplayer-1]->name << " enter column to put disc in (or enter 83 to save the game and return to menu): ";
            cin >> user_input;
            clearBadInput();
        }
        else
            user_input = rand()%board.total_columns+1; //randomizer chooses random value
        
        if (user_input==83) //if saving a loaded game again
        {
            time_t end = time(0);
            double seconds = difftime(end,start);
            players[0]->time += seconds;
            players[1]->time += seconds;
            return 1; //re-saving is handled in loadGame function
        }
        while((user_input>board.total_columns || user_input<1) && user_input!=83) //if user gives invalid column number
        {
            cout << "Invalid column number!\n";
            cout << players[currplayer-1]->name << " enter column to put disc in (or enter 83 to save the game and return to menu): ";
            cin >> user_input;
            clearBadInput();
        }

        int added = board.addToColumn(user_input,currplayer);
        while(added==0){ // if the column is full
            if(players[currplayer-1]->name!="The Creators"){
                cout << "This column is full.\n";
                cout << players[currplayer-1]->name << " enter column to put disc in: ";
                cin >> user_input;
                clearBadInput();
            }
            else
                user_input = rand()%board.total_columns+1;
            added = board.addToColumn(user_input,currplayer);
        }
        players[0]->turns++;
        players[1]->turns++;
        firstturn = false; //if turn happens then first turn obviously over

        int won = board.winCondition(user_input,currplayer);
        if(won)
        {
            board.display(); //display winning board
            cout << '\n' << players[currplayer-1]->name << " won the game! Congratulations!!\n";
            time_t end = time(0);
            double seconds = difftime(end,start);
            cout << "That game took " << players[currplayer-1]->time + seconds << " seconds\n";
            players[currplayer-1]->time += seconds;
            

            //ask to save in leaderboard, and if yes, then save
            if(players[currplayer-1]->name!="The Creators")
            {
                players[currplayer-1]->score = 30000/(players[currplayer-1]->time+5*players[currplayer-1]->turns); //arbritrary score formula we came up with
                cout << "\nYou got a score of " << players[currplayer-1]->score << endl;

                int choice;

                do{
                cout << "Would you like to save this record to the leaderboard? (1 for Yes, 0 for No): ";
                cin >> choice;
                clearBadInput();
                }while(choice<0||choice>1);
                
                cout << "\n";
                if(choice){
                    Leaderboard leader(board.n,(game_against_ai?1:2)); //pass the n for the board and pass 1 if 1p or 2 if not
                    int madeleaderboard = leader.makeVector();
                    leader.updateLeaderboard(*players[currplayer-1]);
                    leader.saveLeaderboard();
                }
            }
            endgame=true;
        }
        else
        {
            currplayer = 3-currplayer;
        }

        if(!won && board.drawCondition()){
            board.display();
            cout << "\nThe game has ended in a draw! The board is full. Better luck next time\n";
            time_t end = time(0);
            double seconds = difftime(end,start);
            cout << "That game took " << players[currplayer-1]->time + seconds << " seconds\n";
            endgame=true;
        }
        
    }while(!endgame);
    return 0;
}

void loadGame()
{
    string name1, name2;
    int n,turns,time;
    int counter=0;
    int toload;
    int game_status = 0;

    vector<Board> boards;
    vector<Player> players;

    fstream saves("gamesaves.txt",ios::in); //read from save file

    if(!saves.is_open()){ //error if it doesn't exist
        cout << "There is no game saves file.\n";
        Sleep(3000);
    }
    else
    {
        while (saves >> n >> name1 >> name2 >> turns >> time) //put all saved games into vectors
        {
            Board tempboard(n);
            for(int i=0;i<n+2;i++)
                for(int j=0;j<n+3;j++)
                    saves >> tempboard.grid[i][j];
            tempboard.calculateFilledState(); //populate filled array of created board

            players.push_back(Player(name1,1,turns,0,time));
            players.push_back(Player(name2,2,turns,0,time));
            boards.push_back(tempboard);
            counter++;
            //put players and board into the vectors and increment counter
        }

        if(counter==0){ //empty or error
            cout << "No valid saved games found.\n";
            Sleep(3000);
            return;
        }


        cout << "=================================\n            GAME SAVES\n=================================\n";
        int gameindex = 1;
        for(int x=0;x<2*counter;x+=2) //display all current saved games and let the user choose
        {
            cout << gameindex << ". "<< players[x].name << ", " << players[x+1].name << ", Connect" << boards[gameindex-1].n <<endl;
            gameindex++;
        }
        cout << "What game would you like to load?(Enter an out of range number to exit) ";
        cin >> toload;
        clearBadInput();
        if (toload < 1 || toload > counter) {
            cout << "Invalid game selection. Returning to menu.\n"; //also gives a way to get out of the load game menu
            Sleep(1000);
            return;
        }
        toload--; //to load is now equal to index of board in the vector
        gameindex = 2*toload; //game index is now index of player 1 in vector

        saves.close();

        //play resumed game
        game_status = resumeGame(players[gameindex], players[gameindex+1],boards[toload]);
        
        //remove the loaded game from the vectors and make the file again    
        ofstream override("gamesaves.txt");
        int playerindex=0;
        for(int boardindex=0;boardindex<counter;boardindex++)
        {
            if(boardindex==toload && game_status==0) //only skip file if wasn't resaved. If was it would have updated the board and players properly.
                continue;
                //if game was saved from loadGame function then gamestatus would be 1 and players and board will have been updated correctly
            else
            {
                playerindex = 2*boardindex;
                override << boards[boardindex].n << " " << players[playerindex].name << " " << players[playerindex+1].name << " " << players[playerindex].turns << " " << players[playerindex].time;
                for(int i=0;i<boards[boardindex].total_rows;i++)
                    for(int j=0;j<boards[boardindex].total_columns;j++)
                        override << " " << boards[boardindex].grid[i][j];
                override << '\n';
            }
        }

        override.close();

    }
}
