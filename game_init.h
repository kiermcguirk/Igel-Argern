/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   newfile.h
 * Author: lpasqua
 *
 * Created on 06 March 2019, 12:18
 */

//number of rows of the board
#define NUM_ROWS 6
//number of columns of the board
#define NUM_COLUMNS 9
#define MAX_NAME_LEN 30

//types of squares
enum stype{
    NORMAL, OBSTACLE };

//colors of tokens
enum color {
    RED, BLUE, GREEN, YELLOW, PINK, ORANGE
};

//defines a token. 
//Note each token can be associated with a colour
typedef struct token
{
   enum color col; //enum color col attribute
   struct token *next; //Pointer to next address in the stack
}token;

//Defines a square of the board.
typedef struct square{
    //A square can be a NORMAL or an OBSTACLE square
     enum stype type;
     //the stack of tokens that can be placed on the board square
     token * stack;
     int numTokens; //number of tokens integer attribute
}square;


//Structure for player
typedef struct player{
    char username[MAX_NAME_LEN]; //Char username attribute
    enum color col; //enum color col attribute
    int winningTokens; //int winning tokens attribute
}player; 


/*
 * This function creates the board for the first time
 * 
 * Input: board - a 6x9 array of squares
 *
 */
void initialize_board(square board[NUM_ROWS][NUM_COLUMNS]);

/*
 * This function creates players for the first time
 * 
 * Input: the array of players to be initialized
 * Output: The number of players of the game
 *
 */
int initialize_players(player players[]);



