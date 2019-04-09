/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include "game_init.h"
#include <stdio.h>


/*
 * This function creates the board for the first time
 * 
 * Input: board - a 6x9 array of squares
 *
 */
void initialize_board(square board[NUM_ROWS][NUM_COLUMNS]){
   
    for (int i =0; i< NUM_ROWS; i++){
        for(int j =0; j < NUM_COLUMNS; j++){
            //creates an obstacle square at positions (0,3), (1,6), (2,4), (3,5), (4,2) and (5,7)
            if((i == 0 && j==3) || (i == 1 && j == 6) || (i ==2 && j ==4) 
                    || (i == 3 && j ==5) || (i==4 && j==2) || (i==5 && j==7)){
                board[i][j].type = OBSTACLE;
            } else{
                //creates a normal square otherwise
                board[i][j].type = NORMAL;
            }
            board[i][j].stack = NULL;
        }
    }
}
    
    
 /*
 * This function creates players for the first time
 * 
 * Input: the array of players to be initialized
 * Output: The number of players of the game
 */
int initialize_players(player players[])
{
    //Declare local variables
    char color_array[6]={'R','B','G','Y','P','O'}; //Declare character array of colours that the players can choose from
    int choice; //Declare integer variable to store the user choice
    int player_count = 0; //Initialise integer variable that stores player count to 0
    int next_user=0; //Declare integer variable to act as a boolean for if the user would like to enter more players and initialise it to 0
    
    //For loop from 0 to maximum number of players
    for (int i=0; i<6; i++)
    {   
        //If the current number of players is greater than 1
        if(player_count > 1)
        {   
            //Prompt the user to input whether or not they would like to continue entering players
            printf("\n*Would you like to continue entering players?*\n1: Yes \n2: No");
            printf("\nEnter option: ");
            scanf("%d", &next_user); //Store user choice in integer variable next_user
            
            //While the value entered is not 1 or 2
            while(!(next_user == 1 || next_user == 2))
            {   
                //Notify user that their input is invalid and prompt them to enter a valid value
                printf("\nInvalid Input! Please enter again:\n");
                printf("\nEnter option: ");
                scanf("%d", &next_user);
            }
        }
        //If the user chose not to enter more players
        if(next_user == 2)
        {   
            //Exit for loop
            break;
        } 
        
        //Print horizontal line
        for(int j =0; j<40; j++)
        {
            int c = 196;
            printf("%c", c);
        }
        
        //Prompt user to input the ith player's name
        printf("\nPlayer %d please enter your name:\n", i+1); // i jenniger sucks massively 
        
        fgets(players[i].username,MAX_NAME_LEN, stdin); //Store input in ith username attriute in the players array
        //scanf("%s", players[i].username); 
        fflush( stdout );
        
        //Ask current player to choose a colour
        printf("\n%s, please choose your colour:\n", players[i].username);
        
        //for loop from i =0 to maximum number of players
        for(int i=0; i<6; i++)
        {   
            //If the ith element in the color_array is not 'x'
            if(!(color_array[i] == 'x'))
            {   
                //Output the choosable colour to the user 
                printf("\n~Please enter %d for ",i+1);
                printf("%c \n", color_array[i]);
            }
        }
        //Prompt user to choose a colour and store it in an integer variable, from 1-6
        printf("\nEnter option: ");
        scanf("%d", &choice); 
        
        //While the user's choice is greater than 6 or less than 1
        while (choice > 6 || choice < 1)
        {   
            //Notify the user that their input is invalid and get a valid choice
            printf("Invalid input, please enter a number shown \n");
            printf("\nEnter option: ");
            scanf("%d", &choice);
        }
        //While the user's choice is already in use
        while(color_array[choice-1] == 'x')
        {   
            //Notify user that their choice is occupied and for them to get enter another choice
            printf("Someone has already chosen this colour! Please choose another! \n");
            printf("\nEnter option: ");
            scanf("%d", &choice);
        }
        //Switch expression for the user's choice
        switch(choice)
        {   
            //If 1
            case 1: 
                players[i].col = RED; //Set the ith player's colour to red
                color_array[0] = 'x'; //Set red in the colours array to x to prevent repetition
                break;
            //If 2
            case 2: 
                players[i].col = BLUE; //Set the ith player's colour to blue
                color_array[1] = 'x'; //Set blue in the colours array to x to prevent repetition
                break;
            //If 3    
            case 3: 
                players[i].col = GREEN; //Set the ith player's colour to green
                color_array[2] = 'x'; //Set green in the colours array to x to prevent repetition
                break;
            //If 4    
            case 4: 
                players[i].col = YELLOW; //Set the ith player's colour to yellow
                color_array[3] = 'x'; //Set yellow in the colours array to x to prevent repetition
                break;
            //If 5    
            case 5: 
                players[i].col = PINK; //Set the ith player's colour to pink
                color_array[4] = 'x'; //Set pink in the colours array to x to prevent repetition
                break; 
            //If 6    
            case 6: 
                players[i].col = ORANGE; //Set the ith player's colour to orange
                color_array[5] = 'x'; //Set orange in the colours array to x to prevent repetition
                break;             
        }
        player_count++; //Increment the player count
    }
    //Print horizontal line
    for(int j =0; j<40; j++)
    {
        int c = 196;
        printf("%c", c);
    }
    printf("\n"); //Print new line
    
    //Output the players and their selected colours to the user
    for(int i=0; i<player_count; i++)
    {
        printf("%s \t %d\n", players[i].username, players[i].col);
    }
    
    //Return the value of player count to the main function
    return player_count;    
 }
    
   
     

