/* 
function to run the game in terminal is: 
g++ connectN_main.cpp connectN_functions.cpp -o connectNgame; .\connectNgame 
*/

#include "connectN.hpp" //include header file

int main()
{
    srand(time(0)); //get different randomization for every time program runs
    int userchoice;
    int amountplayers;

    do{
        userchoice = showMenu();
        

        switch (userchoice)
        {
        case 1:{
            cout << "=======================================================\n                       HOW TO PLAY\n=======================================================\n" << endl;
            cout << "1. Choose the grid size you want to play with and \n   the number of players you are playing with.\n" << endl;
            cout << "2. If you play with a 6x7 grid, you will need to \n   make a line of 4 to win, and if you play with a \n   7x8 grid, you will need to make a line of 5 to win.\n" << endl;
            cout << "3. Players take turns dropping one of their discs \n   into any of the open columns at the top of the grid.\n" << endl;
            cout << "4. The disc falls to the lowest available space in \n   that column.\n" << endl;
            cout << "5. The first player to connect four or five of their\n   own discs in a line (horizontally, vertically, or \n   diagonally) wins the game.\n" << endl;
            Sleep(6000);
            break;}
        case 2:{
            Board game_board(4);
            cout << "Do you want to play with 2 players or 1 player against the program? Enter 2 or 1 respectively: ";
            cin >> amountplayers;
            clearBadInput(); 
            
            while(amountplayers<1 || amountplayers>2){
                cout << "Please enter 1 or 2\n";
                cout << "Do you want to play with 2 players or 1 player against the program? Enter 2 or 1 respectively: ";
                cin >> amountplayers;
                clearBadInput();
                
            }
            if(amountplayers==2)
                playGame2P(game_board);
            else
                playGame1P(game_board);
            
            break;}
        
        case 3:{
            Board game_board(5);
            cout << "Do you want to play with 2 players or 1 player against the program? Enter 2 or 1 respectively: ";
            cin >> amountplayers;
            clearBadInput();
            
            while(amountplayers<1 || amountplayers>2){
                cout << "Please enter 1 or 2\n";
                cout << "Do you want to play with 2 players or 1 player against the program? Enter 2 or 1 respectively: ";
                cin >> amountplayers;
                clearBadInput();
                
            }
            if(amountplayers==2)
                playGame2P(game_board);
            else
                playGame1P(game_board);

            break;}

        case 4:{
            loadGame();
            break;}

        case 5:{
            int listchoice,playerchoice,boardchoice,leaderlistchoice;
            cout << "Which leaderboard would you like to view?\n1. Connect 4, 1 Player\n2. Connect 4, 2 Player\n3. Connect 5, 1 Player\n4. Connect 5, 2 Player\n";
            cout << "Enter your choice: ";
            cin >> leaderlistchoice;
            clearBadInput();
            
            cout << "\n";
            while(leaderlistchoice<1||leaderlistchoice>4)
            {
                cout << "Number not in range!! Invalid input\n";
                cout << "Which leaderboard would you like to view?\n1. Connect 4, 1 Player\n2. Connect 4, 2 Player\n3. Connect 5, 1 Player\n4. Connect 5, 2 Player\n";
                cout << "Enter your choice: ";
                cin >> leaderlistchoice;
                clearBadInput();

                cout << "\n";
            }
            switch(leaderlistchoice)
            {
                case 1:
                    boardchoice=4; playerchoice=1;
                    break;

                case 2:
                    boardchoice=4; playerchoice=2;
                    break;

                case 3:
                    boardchoice=5; playerchoice=1;
                    break;

                case 4:
                    boardchoice=5; playerchoice=2;
                    break;
            }

            Leaderboard leader(boardchoice,playerchoice);
            int exists = leader.makeVector();
            if (exists){
                cout << "How many elements do you want to view?\n1. Top 15\n2. All High Scores\nEnter your choice: ";
                cin >> listchoice;
                clearBadInput();
                
                cout << "\n";
                while(listchoice<1||listchoice>2)
                {
                    cout << "Number not in range!! Invalid input\n";
                    cout << "How many elements do you want to view?\n1. Top 15\n2. All High Scores\nEnter your choice: ";
                    cin >> listchoice;
                    clearBadInput();
                    
                    cout << "\n";
                }

                switch (listchoice)
                {
                case 1:
                    leader.displayLeaderboard(15);
                    Sleep(3000); //stall for a bit so the user can actually read the leaderboard before the menu reappears
                    break;
                case 2:
                    leader.displayLeaderboard(-1); //neg number will output all
                    Sleep(3000);
                    break;
                default:
                    break;
                }
                
            }
            else{
                cout << "\nFILE NOT FOUND. PLEASE CREATE A LEADERBOARD FILE.\n";
                Sleep(3000);
            }
            break;}

        case 6:{
            cout << "\nGoodbye! See you later\n";
            break;}
        }
        
    }while(userchoice!=6);



    return 0;
}