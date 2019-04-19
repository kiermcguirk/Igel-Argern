#include "game_init.h"
#include "game_logic.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

void printLine();

char print_token(token *t)
{
    if((*t).col== PINK) return 'P';
    if((*t).col== RED) return 'R';
    if((*t).col== BLUE) return 'B';
    if((*t).col== GREEN) return 'G';
    if((*t).col== ORANGE) return 'O';
    if((*t).col== YELLOW) return 'Y';
    return '\0';
}

void print_board(square board[NUM_ROWS][NUM_COLUMNS]){
    printf("                THE BOARD\n");
    for(int i =0; i < NUM_ROWS; i++){
       
        //prints an horizontal line
        printLine();
        //prints the row number
        printf(" %d ", i);
        char c = '\0' ;
        //if the square (i,j) is occupied,
        //c is assigned the initial of the color of the token that occupies the square
        for (int j = 0; j < NUM_COLUMNS; j++){
            if(board[i][j].stack != NULL){
                c = print_token(board[i][j].stack);
            }
            //if the square (i,j) is empty
            else{
                //c is assigned 'X' if the square represents an obstacle
                if(board[i][j].type == OBSTACLE)
                    c = 'X';
                //c is assigned an empty space otherwise
                else c = ' ';
            }
            printf("| %c ", c);
        }
        printf("|\n");
    }
    printLine();
    //prints the number of the columns at the end of the board
    printf("     0   1   2   3   4   5   6   7   8\n");
}

void printLine(){
  printf("   -------------------------------------\n");  
}

void place_tokens(square board[NUM_ROWS][NUM_COLUMNS], player players[], int numPlayers)
{   
    //The min number of tokens placed on a square in the first column of the board
    int minNumOfTokens = 0;
    int selectedSquare = 0;
    
    for(int i=0; i<6; i++)
    {
      board[i][0].numTokens = 0;
    }
    
    for(int i=0; i<4; i++)
    {
        
        for(int j=0; j<numPlayers; j++)
        {   
            printf("Player %d please select a square (0-5): ", j+1);
            scanf("%d", &selectedSquare);
            printf("\n");
            designline();
            
            
            while((board[selectedSquare][0].numTokens != minNumOfTokens) || (checkforcolour(board[selectedSquare][0], players[j])))
            {
             
             printf("You cannot place a token here as it doesn't have the minimum number of tokens or because it is already occupied by your colour\n");
             printf("Player %d please select another square\n",j);
             scanf("%d", &selectedSquare);
             designline();
            }
            
            struct token *c = board[selectedSquare][0].stack;
            board[selectedSquare][0].stack = (token *)malloc(sizeof(token));
            board[selectedSquare][0].stack->col = players[j].col;
            if(board[selectedSquare][0].numTokens >0)
            {   
                board[selectedSquare][0].stack->next = c;
            }
            board[selectedSquare][0].numTokens++;
            
            print_board(board);
            designline();
            if(((numPlayers * i)+j+1)%NUM_ROWS ==0)
            {
                minNumOfTokens++;
            }
        }
    }
}


void play_game(square board[][NUM_COLUMNS], player players[], int numPlayers)
{   
    //print_board(board);
    printf("~The game will now begin!~\n");
    int winner = 0;
    //Seed random number function
    srand(time(NULL));
    while(winner == 0)
    {   
        
        for(int i=0; i<numPlayers; i++)
        {  
           
           designline();
           printf("~Player %d has rolled the dice!~\n", i+1);
           int throw = rand()%6; // returns an int in the range 0..5
      
           struct player *playerPtr;
           playerPtr = &players[i];
                     
           printf("*Player %d has rolled: %d*\n", i+1, throw);
           designline();
           printf("~Player %d has the option to sidestep one of their tokens!~\n",i+1);
           designline();
          // check_board(board, playerPtr);
           designline();
           printf("~Player %d must now move one of the tokens in row %d forward!~\n",i+1, throw);
           move_token_forward(board, throw, playerPtr);
           if(checkWinner(players[i]))
           {
               winner =1;
               break;
           }
           
        }  
    }
    credits();
    
}

void move_token_forward(square board[][NUM_COLUMNS], int throw, struct player *player)
{   
   // loop through all squares in row[throw]
   // if row[throw] square (top of stack token col) == player col
   // then ask player if wants to move token. 
           
   int playerMovedToken = 0;  
   int foundTokens =0;
   
   while(playerMovedToken == 0)
   {
        for (int i=0; i<NUM_COLUMNS-1; i++)
        {    
            if(playerMovedToken == 1)
            {
                break;
            }
            else if(board[throw][i].stack != NULL && playerMovedToken == 0)
            {   
                foundTokens++;
                if(board[throw][i].type == OBSTACLE)
                 {
                     if(checkobstacle(board,i))
                     {
                         printf("\nNOTE: The token on the following obstacle square can be moved as there is a token in a column behind it\n");
                     }
                     else
                     {   
                         printf("\nNOTE: The token at [%d,%d] cannot be moved forward as there is no token in a column behind it\n",throw,i);
                         continue;
                     }
                 }

                printf("*Found a token in square [%d,%d], would you like to move it?*", throw, i);

                if (yesorno()) 
                { // move a player token to next right square
                    playerMovedToken = 1;
                    enum color *colourPtr;
                    colourPtr = &board[throw][i].stack->col;              
                    board[throw][i+1] = push(&board[throw][i+1],colourPtr);
                    board[throw][i] = pop(&board[throw][i]);                              
                    print_board(board);
                    
                    if(i == 8)
                    {   
                        printf("im here");
                        (*player).numTokensLastCol++;
                    }
                }
            }
        } // for loop
        if(foundTokens == 0)
        {
            printf("~Oops! There are no tokens on this row to move forward! You will have to roll the dice again!~\n");
            designline();
            
            throw = rand()%6;
            printf("*The new dice roll is: %d* \n", throw);
        }
        else if(playerMovedToken == 0 && foundTokens > 0)
        {    
            printf("~REMINDER: You MUST move a token in row %d! We will ask you again...\n", throw);
            designline();
        }
    }//end while
}



void check_board(square board[][NUM_COLUMNS],player *player)
{   
    int sidestep =0;
    for(int i=0; i<NUM_ROWS; i++)
    {   
        
        if(sidestep==1)
        {
            break;
        }
         
        for(int j=0; j<NUM_COLUMNS; j++)
        {   
            if(sidestep == 1)
            {   
                break;
            }
            if(board[i][j].stack == NULL)
            {
                continue;
            }
                
            else if(board[i][j].numTokens >0)
            {   
                if(board[i][j].stack->col == (*player).col)
                {
                    if(board[i][j].type == OBSTACLE)
                    {
                        if(checkobstacle(board,j)) 
                        {
                            printf("\nThe token on the following obstacle square can be sidestepped as there is a token in a column behind it\n");
                        }
                        else
                        {   
                            printf("\nThe token at [%d,%d] cannot be sidestepped as there is no token in a column behind it\n",i,j);
                            continue;
                        }
                    }

                    printf("You have a token at [%d,%d], would you like to sidestep?", i,j);
                    if(!(yesorno()))
                    {
                        continue;
                    }
                    else
                    {   
                        if(!((i-1)<0))
                        {
                            printf("Would you like to move the token to [%d, %d]?", i-1, j);
                            if(yesorno())
                            {   
                                enum color *colourPtr;
                                colourPtr = &(*player).col;
                                board[i-1][j] = push(&board[i-1][j], colourPtr);
                                board[i][j] = pop(&board[i][j]);
                                print_board(board);
                                sidestep=1;
                                break;
                            }
                        }
                         
                        printf("\n Would you like to move the token to [%d, %d]?", i+1, j);
                        if(yesorno())
                        {   
                            enum color *colourPtr;
                            colourPtr = &(*player).col;
                            board[i+1][j] = push(&board[i+1][j], colourPtr);
                            board[i][j] = pop(&board[i][j]);
                            print_board(board);
                            sidestep=1;
                            break;
                        }   
                    }
                }
            }
        }
    }
}


struct square push(struct square *top, enum color *colourPtr)
{
    struct token *c = (*top).stack;
    
    (*top).stack = (token*)malloc(sizeof(token));
    (*top).stack->col = *colourPtr;
    (*top).stack->next = c;
    (*top).numTokens++;
        
    return *top;
}

struct square pop(struct square *top)
{
    struct token *c = (*top).stack;
    if(c!=NULL)
    {   
        
        if((*top).numTokens >1)
        {
          (*top).stack = c->next; 
        }
        
        if((*top).numTokens == 1)
        {
          (*top).stack = NULL;
        }
        //(*top).stack = c->next; 
        (*top).numTokens--;
        free(c); 
    }
    return *top;
    
}

bool checkobstacle(square board[NUM_ROWS][NUM_COLUMNS], int index)
{   
    //For loop which goes through each row, in the column marked 'index' which is passed as a parameter
    for(int i=0; i<NUM_ROWS; i++)
    {   
        //If there exists a non-empty stack at row i, index-1
        if(board[i][index-1].stack != NULL)
        {   
            //return true
            return true; 
        }
    }
    //Return false
    return false;
}

void designline()
{
    //Print horizontal line
    for(int j =0; j<40; j++)
    {
        int c = 196;
        printf("%c", c);
    }
    printf("\n"); //Print new line
}

bool yesorno()
{
    int choice;
    printf("\n1: Yes \n2: No");
    printf("\nEnter option: ");
    scanf("%d", &choice);
    while(choice != 1 && choice != 2)
    {   
        printf("\nThat is not a correct input! Try again:\n");
        printf("\nEnter option: ");
        scanf("%d",&choice);
    }
    switch(choice)
    {
        case 1:
            designline();
            return true;
            //break;
        case 2:
            designline();
            return false;
           // break;
    }
}

bool checkWinner(struct player selectedPlayer)
{
    if(selectedPlayer.winningTokens == 3)
    {
        return true;
    }
    else
    {
        return false;
    }
    
}

bool checkforcolour(struct square selectedSquare, struct player selectedPlayer)
{
    if (selectedSquare.stack == NULL)
    {
        return false;
    }
    else if(selectedSquare.stack->col == selectedPlayer.col)
    {
        return true;
    }
    return false;
}

void credits()
{
    //clear screen
    system("@cls||clear");
    
    printf("-------******-------Project By Kier McGuirk(18752609) and Jennifer Flanagan(18422084)-------******-------\n");
    printf("\nWe hope you enjoyed our interpretation of the game Igel Argern! Give us an A+. Brian Lamont is not a grass\n");
    printf("\nFirst round on you to celebrate at the student bar!\n");
    
}

