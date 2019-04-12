/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: lpasqua
 *
 * Created on 06 March 2019, 12:11
 */

#include "game_init.h"
#include "game_logic.h"
#include <stdio.h>



/*
 * 
 */
int main(int argc, char** argv) {
    //clear screen
    system("@cls||clear");
    
    printf("-------******-------Project By Kier McGuirk(18752609) and Jennifer Flanagan(18422084)-------******-------\n");
    
    //the board is defined as a 2-Dimensional array of squares
    square board[NUM_ROWS][NUM_COLUMNS];
    
    //an array containing the players (MAX 6 players)
    player players[6];
    
    //the number of players
    int numPlayers =0;
    
    //creates the squares of the board
    initialize_board(board);
    
    //prints the board
    print_board(board);
    
    //creates the players
    numPlayers = initialize_players(players);
    
    //asks each player to place their tokens
    //on the first column of the board
    place_tokens(board, players, numPlayers);
    
    
    //manages the turns of the game and identifies a winner
//   play_game(board, players, numPlayers);
    
    return 0;
    
    

}

