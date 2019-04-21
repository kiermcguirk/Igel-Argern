#include <stdbool.h>

//Declare function prototypes
void print_board(square board[NUM_ROWS][NUM_COLUMNS]);

void place_tokens(square board[NUM_ROWS][NUM_COLUMNS], player players[], int numPlayers);

void play_game(square board[NUM_ROWS][NUM_COLUMNS], player players[], int numPlayers);

void check_board(square board[NUM_ROWS][NUM_COLUMNS],player players[]);

void move_token_forward(square board[NUM_ROWS][NUM_COLUMNS], int throw, struct player *player);

struct square push(struct square *top, enum color *colourPtr);

struct square pop(struct square *top);

bool checkobstacle(square board[NUM_ROWS][NUM_COLUMNS], int index_j, int index_i);

void designline();

bool yesorno();

void credits();

bool checkWinner(struct player selectedPlayer);

bool checkforcolour(struct square selectedSquare, struct player selectedPlayer);

void printLine();



