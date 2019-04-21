#include "game_init.h"
#include "game_logic.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>


/*
 *Function to print the colour of token
 *Returns a character and takes a pointer to a token as an argument
 */
char print_token(token *t)
{   
    
    //Return the first letter corresponding to the token's colour
    if((*t).col== PINK) return 'P'; //If pink return 'p'
    if((*t).col== RED) return 'R'; //If red return 'r'
    if((*t).col== BLUE) return 'B'; //If blue return 'b'
    if((*t).col== GREEN) return 'G';  //if green return 'g'
    if((*t).col== ORANGE) return 'O'; //if orange 'o'
    if((*t).col== YELLOW) return 'Y'; //if yellow return 'y'
    return '\0'; //else return null terminator
}
/*
 *Function to print the current status of the board
 *Returns void and takes the full board as an argument
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
        //c is assigned the initial of the colour of the token that occupies the square
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
/*
 *Function to print line when printing the board
 *Returns void and has void arguments
 */
void printLine()
{
  //print dashed line
  printf("   -------------------------------------\n");  
}
/*
 *Function to place tokens on the leftmost column of the board, the players array and the number of players
 *Returns void and has void arguments
 */
void place_tokens(square board[NUM_ROWS][NUM_COLUMNS], player players[], int numPlayers)
{   
    //Declare local variables
    int minNumOfTokens = 0; //The min number of tokens placed on a square in the first column of the board
    int selectedSquare = 0; //Initialise selectedSquare to 0
    
    //For loop from 0 to 6
    for(int i=0; i<6; i++)
    { 
      //inner for loop from 0 to NUM_COLUMNS
      for(int j =0; j<NUM_COLUMNS;j++)
      {   
          //initialise current square's number of tokens to zero and its stack to null
          board[i][j].numTokens = 0;
          board[i][j].stack = NULL;
      }
    }
    
    //for loop from 0-4
    for(int i=0; i<4; i++)
    {   
        //inner for loop from 0-numPlayers
        for(int j=0; j<numPlayers; j++)
        {   
            //Prompt current player to select a square to which they would like to place their token
            printf("Player %d please select a square (0-5): ", j+1);
            scanf("%d", &selectedSquare);
            printf("\n"); //Print newline
            designline(); //Call design line
            
            //While the selected square's number of tokens is not the minimum number of tokens or if the square's top token's colour is the same as the current players
            while((board[selectedSquare][0].numTokens != minNumOfTokens) || (checkforcolour(board[selectedSquare][0], players[j])))
            {
                //Notify user that they cannot place a token here and ask them for another square
                printf("You cannot place a token here as it doesn't have the minimum number of tokens or because it is already occupied by your colour\n");
                printf("Player %d please select another square\n",j);
                scanf("%d", &selectedSquare);
                designline(); //Call design line
            }
            
            //Make a token pointer to point to the current square's top token
            struct token *c = board[selectedSquare][0].stack;
            board[selectedSquare][0].stack = (token *)malloc(sizeof(struct token)); //Increase stack's memory by size of struct token
            board[selectedSquare][0].stack->col = players[j].col; //Set the top of the stack to the players token colour
            
            //If the selected square already has a token
            if(board[selectedSquare][0].numTokens >0)
            {   
                //Set the pointer to the next token to the token pointer
                board[selectedSquare][0].stack->next = c;
            }
            board[selectedSquare][0].numTokens++; //Increment the square's numbe of tokens
            
            print_board(board); //Print board
            designline(); //Call design line
            
            //When leftmost column is full of tokens increment minNumofTokens by 1
            if(((numPlayers * i)+j+1)%NUM_ROWS ==0)
            {
                minNumOfTokens++;
            }
        }
    }
}

/*
 *Function to begin the game and allows the players to play
 *Returns void and takes the full board by reference, the players array and the number of players as arguments
 */
void play_game(square board[][NUM_COLUMNS], player players[], int numPlayers)
{   
    //Notify players that the game will begin
    printf("~The game will now begin!~\n");
    int winner = 0; //Declare winner variable to start a loop until a winner is found
    
    //Seed random number function
    srand(time(NULL));
    
    //While winner has not been found
    while(winner == 0)
    {   
        //For loop from 0 to number of players-1
        for(int i=0; i<numPlayers; i++)
        {  
           designline();//Print designline
           printf("~Player %d has rolled the dice!~\n", i+1); //Roll dice for current user
           int throw = rand()%6; // returns an int in the range 0..5 for the dice throw
           
           //Create a player pointer to point to the current player
           struct player *playerPtr;
           playerPtr = &players[i];
                     
           printf("*Player %d has rolled: %d*\n", i+1, throw); //Notify player what they have thrown
           designline(); //Call designline
           printf("~Player %d has the option to sidestep one of their tokens!~\n",i+1); //Notify player they can sidestep one of their tokens
           designline(); //Call design line
           check_board(board, playerPtr); //Call checkboard function passing the board, and the player pointer as arguments
           designline(); //Call design line
           printf("~Player %d must now move one of the tokens in row %d forward!~\n",i+1, throw); //Notify user that they must move a token in the row that corresponds to the dice
           move_token_forward(board, throw, playerPtr); //Call move_token_forward function and pass the board, the dice throw, and the player pointer as arguments
           
           //Check to see if the current player has won the game
           if(checkWinner(players[i]))
           {
               winner =1; //Set winner variable to 1
               break; //Break from loop
           }
        }  
    }
    credits(); //Call credits function
}
/*
 *Function to move a hedgehog forward on the board
 *Returns void and but takes the full board of squares, the dice roll and the current player as arguments
 */
void move_token_forward(square board[][NUM_COLUMNS], int throw, struct player *player)
{   
   //Declare local variables
   int playerMovedToken = 0; //variable to check if player has moved
   int foundTokens =0; //variable to check to count how many tokens can be moved
   
   //While the player hasn't moved a token
   while(playerMovedToken == 0)
   {    
        //for loop from 0 to NUM_COLUMNS -2
        for (int i=0; i<NUM_COLUMNS-1; i++)
        {   
            //If player has moved
            if(playerMovedToken == 1)
            {   
                //break loop
                break;
            }
            //Else if the current square's stack is not null and the player hasn't moved
            else if(board[throw][i].stack != NULL && playerMovedToken == 0)
            {   
                foundTokens++; //Increment foundTokens
                //If the current square is an obstacle
                if(board[throw][i].type == OBSTACLE)
                 {   
                     //Call checkobstacle function to see if the token on the obstacle square can be moved
                     if(checkobstacle(board,i,throw))
                     {  
                         //notify user that they can move the token
                         printf("\nNOTE: The token on the following obstacle square can be moved as there is a token in a column behind it, or because there is a token in the same column, or a column ahead\n");
                     }
                     else
                     {   
                         //notify the player that they cannot move the token
                         printf("\nNOTE: The token at [%d,%d] cannot be moved forward as there is no token in a column behind it or there is no token in the same column or a column ahead\n",throw,i);
                         continue; //Continue to next iteration of loop
                     }
                 }
                //Ask player if they would like to move the token
                printf("*Found a token in square [%d,%d], would you like to move it?*", throw, i);
                
                //If user wishes to move token
                if (yesorno()) 
                {   //move a player token to next right square
                    playerMovedToken = 1;
                    
                    enum color *colourPtr; //Create pointer to the colour of the current square
                    colourPtr = &board[throw][i].stack->col;              
                    board[throw][i+1] = push(&board[throw][i+1],colourPtr); //call push function to add the token to the next square
                    board[throw][i] = pop(&board[throw][i]); //call pop function to delete token from current stack                         
                    print_board(board); //print board
                    
                    //If current player moves a token to the last column
                    if((i+1) == 8)
                    {   
                        (*player).winningTokens++; //increment their winning tokens
                        printf("%s is currently on %d points!\n", (*player).username, (*player).winningTokens); //Display their current winning tokens to player
                    }
                }
            }
        } // for loop
        //If no tokens are found
        if(foundTokens == 0)
        {   
            //Notify player there are no found tokens
            printf("~Oops! There are no tokens on this row to move forward! You will have to roll the dice again!~\n");
            designline(); //Call designline
            
            //Reroll dice
            throw = rand()%6;
            printf("*The new dice roll is: %d* \n", throw);
        }
        //else if the player hasn't moved and there are found tokens
        else if(playerMovedToken == 0 && foundTokens > 0)
        {   
            //Notify player that they must move
            printf("~REMINDER: You MUST move a token in row %d! We will ask you again...\n", throw);
            designline(); //Call designline
        }
    }//end while
}

/*
 *Function to check through the board to see if a player has a hedgehog to sidestep
 *Returns void but takes the full board of squares and the current player as arguments
 */
void check_board(square board[][NUM_COLUMNS],player *player)
{   
    //Declare local variables
    int sidestep =0; //Variable to check if a player has sidestepped
    
    //for loop from 0 to NUM_ROWS-1
    for(int i=0; i<NUM_ROWS; i++)
    {   
        //If the player has sidestepped
        if(sidestep==1)
        {   
            //Break from loop
            break;
        }
         //Inner loop from 0 to NUM_COLUMNS -1
        for(int j=0; j<NUM_COLUMNS; j++)
        {   
            //If the player has sidestepped
            if(sidestep == 1)
            {   
                //Break from loop
                break;
            }
            //If the current square's stack is empty
            if(board[i][j].stack == NULL)
            {   
                //Continue to next iteration of loop
                continue;
            }
            //Else if current square's stack is not empty
            else if(board[i][j].numTokens >0)
            {   
                //If current stack's top token's colour is the players colour
                if(board[i][j].stack->col == (*player).col)
                {   
                    //If the square is an obstacle square
                    if(board[i][j].type == OBSTACLE)
                    {   
                        //If the player can sidestep from the obstacle square
                        if(checkobstacle(board,j, i)) 
                        {   
                            //Notify player that they can move from the obstacle square
                            printf("\nThe token on the following obstacle square can be sidestepped as there is a token in a column behind it, or there is a token in the same column or a column ahead\n");
                        }
                        //else
                        else
                        {   
                            //Notify the player that they cannot move from this square and continue from next loop iteration
                            printf("\nThe token at [%d,%d] cannot be sidestepped as there is no token in a column behind it, or because there is no token in the same column or a column ahead\n",i,j);
                            continue;
                        }
                    }
                    //Ask user if they would like to sidestep token
                    printf("You have a token at [%d,%d], would you like to sidestep?", i,j);
                    //If no
                    if(!(yesorno()))
                    {   
                        //Continue from next iteration of loop
                        continue;
                    }
                    //else
                    else
                    {   
                        //Ensure they can't sidestep off the board
                        if(!((i-1)<0))
                        {   
                            //Ask if they would like to move the token to adjacent position
                            printf("Would you like to move the token to [%d, %d]?", i-1, j);
                            if(yesorno()) //If yes
                            {   
                                //Call push and pop, print the board and set sidestep to 1
                                enum color *colourPtr;
                                colourPtr = &(*player).col;
                                board[i-1][j] = push(&board[i-1][j], colourPtr);
                                board[i][j] = pop(&board[i][j]);
                                print_board(board);
                                sidestep=1;
                                break; //Break from loop
                            }
                        }
                        //Ask user if they would like to move the token to the other adjacent position
                        printf("\n Would you like to move the token to [%d, %d]?", i+1, j);
                        if(yesorno()) //If yes
                        {   
                            //Call push and pop, print board and set sidestep to 1
                            enum color *colourPtr;
                            colourPtr = &(*player).col;
                            board[i+1][j] = push(&board[i+1][j], colourPtr);
                            board[i][j] = pop(&board[i][j]);
                            print_board(board);
                            sidestep=1;
                            break; //Break from loop
                        }   
                    }
                }
            }
        }
    }
}
/*
 *Function to push to a stack
 *Returns a square and takes a pointer to a square and a pointer to a colour as arguments
 */
struct square push(struct square *top, enum color *colourPtr)
{   
    //Set token pointer to the top of the stack
    struct token *c = (*top).stack;
    
    (*top).stack = (token*)malloc(sizeof(struct token)); //Increase stacks memory size by the size of token
    (*top).stack->col = *colourPtr; //Set the top of the stack to the players colour
    
    //If the number of tokens on the stack is not 0
    if((*top).numTokens >0)
    {   
        (*top).stack->next = c; //Set the next pointer to the address stored in the token pointer
    }
    else //else
    {
        (*top).stack->next = NULL; //Set the next pointer to null
    }
    (*top).numTokens++; //Increment the number of tokens
        
    return *top; //return pointer to square
}
/*
 *Function to pop from a stack
 *Returns a square and takes a pointer to a square as an argument
 */
struct square pop(struct square *top)
{   
    //Set token pointer to top of the stack
    struct token *c = (*top).stack;
    
    //If the stack is not null
    if(c!=NULL)
    {   
        if((*top).numTokens >1) //If the square has more than one token
        {
          (*top).stack = c->next;  //Set the top of the stack to the next pointer
        }
        if((*top).numTokens == 1) //If the square has only one token
        {
          (*top).stack = NULL; //Set the top of the stack to null
        }
        (*top).numTokens--; //Decrement numTokens
        free(c); //Free the memory stored in the pointer
    }
    return *top; //Return Square pointer
    
}
/*
 *Function to check if the hedgehog on the current obstacle square can be moved
 *Returns true/false and takes the obstacle square, and indexes i and j as arguments
 */
bool checkobstacle(square board[NUM_ROWS][NUM_COLUMNS], int index_j, int index_i)
{   
    //For loop which goes through each row, in the column marked 'index' which is passed as a parameter
    for(int i=0; i<NUM_ROWS; i++)
    {   
        //If there exists a non-empty stack at row i, index-1
        if(board[i][index_j-1].stack != NULL)
        {   
            //return true
            return true; 
        }
        //For loop from the current position of the obstacle square to NUM_COLUMNS -1
        for(int j = index_j; j<NUM_COLUMNS; j++)
        {   
            //If the current square is not null
            if(board[i][j].stack != NULL)
            {   
                //If the square is the current obstacle square
                if( i==index_i && j == index_j)
                {
                    continue; //Continue from next iteration
                } 
                return true; //Return true
            }
        }
    }
    //Return false
    return false;
}
/*
 *Function to create a line for aesthetic purposes
 *Returns void and has void arguments
 */
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
/*
 *Function prompt the user with the option to choose either yes or no
 *Returns true/false, but has void arguments
 */
bool yesorno()
{   
    int choice; //Declare choice variable
    printf("\n1: Yes \n2: No"); //Prompt user to enter yes or no
    printf("\nEnter option: ");
    scanf("%d", &choice);
    while(choice != 1 && choice != 2) //If value entered is not yes or no notify user to enter a valid value
    {   
        printf("\nThat is not a correct input! Try again:\n");
        printf("\nEnter option: ");
        scanf("%d",&choice);
    }
    switch(choice)
    {
        case 1: //case yes
            designline();
            return true; //return true
            
        case 2: //case no
            designline();
            return false; //return false
    }
}
/*
 *Function to check if a winner has been obtained
 *Returns true/false and takes the selected player as an argument
 */
bool checkWinner(struct player selectedPlayer)
{   
    //If current player has three winningTokens
    if(selectedPlayer.winningTokens == 3)
    {   
        printf("\n %s has won the game!\n", selectedPlayer.username); //Notify user they have won the game
        return true; //Return true
    }
    else //else
    {
        return false; //Return false
    }
    
}
/*
 *Function to check to see if the current square matches a players hedgehog colour
 *Returns true/false, has the square to be compared and the selected player as arguments
 */
bool checkforcolour(struct square selectedSquare, struct player selectedPlayer)
{   
    //If current square is null
    if (selectedSquare.stack == NULL)
    {
        return false; //return false
    }
    else if(selectedSquare.stack->col == selectedPlayer.col) //else if current square's stack colour is the players colour
    {
        return true; //return true
    }
    return false; //return false
}

/*
 *Function to roll credits
 *Returns void and has void arguments
 */
void credits()
{   
    //Declare dummy variable
    int dummy;
    printf("\nPress enter any value to roll the credits\n"); //Notify user to enter any value to roll credits
    ("%d", &dummy);
  
    system("@cls||clear"); //clear screen
    
    //Present banner and credits
    printf("-------******-------Project By Kier McGuirk(18752609) and Jennifer Flanagan(18422084)-------******-------\n");
    printf("\nWe hope you enjoyed our interpretation of the game Igel Argern! \n");
    
}

