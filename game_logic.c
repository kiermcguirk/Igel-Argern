/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


#include "game_init.h"
#include "game_logic.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

void printLine();

/*
 * Returns the first letter associated with the color of the token
 * 
 * Input: t - pointer to a token
 * Output: initial of the color of the token
 */
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

/*
 * Prints the board
 * 
 * Input: the board to be printed. 
 */
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

/*
 * Place tokens in the first column of the board
 * 
 * Input: board - a 6x9 array of squares that represents the board
 *        players - the array of the players
 *        numPlayers - the number of players  
 */
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
            printf("Player %d please select a square", j);
            scanf("%d", &selectedSquare);
            
            
            while((board[selectedSquare][0].numTokens != minNumOfTokens) && (board[selectedSquare][0].stack->col == players[j].col))
            {
             printf("You cannot place a token here as it doesn't have the minimum number of tokens or because it is already occupied by your colour\n");
             printf("Player %d please select another square ",j);
             scanf("%d", &selectedSquare);
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
            if(((numPlayers * i)+j+1)%NUM_ROWS ==0)
            {
                minNumOfTokens++;
            }
        }
    }
        
    
}


/*
 * Place tokens in the first column of the board
 * 
 * Input: board - a 6x9 array of squares that represents the board
 *        players - the array of the players
 *        numPlayers - the number of players  
 */



void play_game(square board[][NUM_COLUMNS], player players[], int numPlayers)
{   
    print_board(board);
    
    printf("\nThe game will now begin!\n");
    int winner = 0;
    int check =0;
    
    printf("Stack top col: %d\n", board[2][0].stack->col);
    printf("Stack next col: %d\n", board[2][0].stack->next->col);
    while(winner == 0)
    {   
        
        for(int i=0; i<numPlayers; i++)
        {
           printf("Player %d has rolled the dice\n", i);
           int throw = rand()%6;
      
           struct player *playerPtr;
           playerPtr = &players[i];
                     
           printf("Player %d has rolled: %d", i, throw);
           check_board(board, playerPtr);
           
        }
    }
}

void check_board(square board[][NUM_COLUMNS],player *player)
{   
    int sidestep =0;
    printf("debugging printf");
    int player_num;
    for(int i=0; i<NUM_ROWS; i++)
    {   
        printf("sidestep is %d", sidestep);
        if(sidestep==1)
        {
            break;
        }
         
        for(int j=0; j<NUM_COLUMNS; j++)
        {   
            if(sidestep == 1)
            {   
                printf("why ament i here");
                break;
            }
            if(board[i][j].stack == NULL)
            {
                continue;
            }
                
            else if(board[i][j].stack->col == (*player).col)
            {
                int choice;
                printf("\nYou have a token at [%d,%d], would you like to move it? Enter 1 for yes, 2 for no\n", i,j);
                printf("Enter option: ");
                scanf("%d", &choice);
                
                if(choice == 2)
                {
                    continue;
                }
                else if(choice == 1)
                {   
                    if(!((i-1)<0))
                    {
                        int choice2;
                        printf("\n Would you like to move the token to [%d, %d]? Enter 1 for yes, 2 for no\n", i-1, j);
                        printf("Enter option: ");
                        scanf("%d", &choice2);
                        
                        if(choice2 == 1)
                        {   
                            //struct player **playerPtr = malloc(sizeof(struct player));
                            //**playerPtr = *player;
                            //struct token **ptr;
                            //ptr = &board[i-1][j].stack;
                            board[i-1][j] = push(&board[i-1][j], player);
                            board[i][j] = pop(&board[i][j]);
                            print_board(board);
                            sidestep=1;
                            break;
                        }
                    }
                    int choice2; 
                    printf("\n Would you like to move the token to [%d, %d]? Enter 1 for yes, 2 for no\n", i+1, j);
                    printf("Enter option: ");
                    scanf("%d", &choice2);
                    
                    if(choice2 == 1)
                    {   
                        //struct token **ptr;
                        board[i+1][j] = push(&board[i+1][j], player);
                        printf("Stack top col: %d\n", board[i][j].stack->col);
                       // printf("Stack next col: %d\n", board[i][j].stack->next->col);
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


struct square push(struct square *top, struct player *player)
{
    printf("Another debugging printf");
    
    struct token *c = (*top).stack;
    
    (*top).stack = (token*)malloc(sizeof(token));
    (*top).stack->col = (*player).col;
    (*top).stack->next = c;
    (*top).numTokens++;
    
    printf("Stack col: %d\n", (*top).stack->next->col);
     
     
    /*
    struct square *c = top;
    
    (*top).stack = (token*)malloc(sizeof(token));
    (*top).stack->col = (*player).col;
    (*top).stack->next = c.stack;
    */
    return *top;
}

struct square pop(struct square *top)
{
    struct token *c = (*top).stack;
    if(c!=NULL)
    {
        (*top).stack = c->next;
        (*top).numTokens--;
        free(c); 
        
    }
    return *top;
    
}



