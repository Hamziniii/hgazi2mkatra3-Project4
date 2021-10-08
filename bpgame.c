// file:  bpgame.c

/** #include statements... **/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bpgame.h"
// i was here
/** TYPEDEFS / STRUCTs HERE ***/
struct bpgame {
   // YOU DECIDE WHAT TO PUT IN HERE TO CAPTURE THE STATE OF
   //   A "RUN" OF THE GAME.....
   int nrows; // number of rows
   int ncols; // number of columns
   int score; 
   char** grid; // the grid itself

   int gridHistorySize;
   char*** gridHistory; // the history of the grid
};

/*** IMPLEMENTATION OF bp_XXXX FUNCTIONS HERE  ****/
extern int isRandInit = 0;

// Sets the seed if it hasnt been set before
void randInit() {
   if(!isRandInit) {
      isRandInit = 1;
      srand((unsigned) time(0));
   }
}

// gets initial value, NOTE: 
BPGame* bp_init(int nrows, int ncols) {
   if(nrows > MAX_ROWS || ncols > MAX_COLS) { // If the rows or columns are greater than 40, error out
      fprintf(stderr, "%s", "Error with nrows (%i) or ncols (%i) in BPGame creation!\n", nrows, ncols);
      return NULL;
   }

   BPGame* game = malloc(sizeof(BPGame)); // allocate space for the game 
   game->nrows = nrows; // sets the rows
   game->ncols = ncols; // sets the cols
   game->gridHistorySize = 5; // set the history size
   game->gridHistory = malloc(sizeof(char) * game->ncols * game->nrows * game->gridHistorySize); // creates the history

   return game;
}

BPGame* bp_create(int nrows, int ncols) {
   randInit();
   BPGame* game = bp_init(nrows, ncols); // gets init of game
   
   char* grid = malloc(sizeof(char) * game->ncols * game->nrows); // create one big line 
   for(int i = 0; i < game->nrows * game->ncols; i++) { // set eaach value to a random "color"
      switch(rand() % 4) { // gets a random number, and sets that point depending on that randome number
         case 0:
            grid[i] = Red;
         case 1: 
            grid[i] = Blue;
         case 2: 
            grid[i] = Green;
         case 3:
            grid[i] = Yellow;
      }
   }

   for(int r = 0; r < game->nrows; r++) // for each row
      game->grid[r] = grid[r * game->nrows]; // set it to the the corresponding place in grid var  
   return game; 
}

// the same thing as above, but getting vlaues from a matrix instead of random
BPGame* bp_create_from_mtx(char mtx[][MAX_COLS], int nrows, int ncols) {
   BPGame* game = bp_init(nrows, ncols); // gets init of game

   char* grid = malloc(sizeof(char) * game->ncols * game->nrows); // create one big line 
   for(int r = 0; r < game->nrows; r++) // for each row
      game->grid[r] = grid[r * game->nrows]; // set it to the the corresponding place in grid var   

   for(int i = 0; i < game->nrows; i++)
      for(int j = 0; j < game->ncols; j++)
         game->grid[i][j] = mtx[i][j]; // for each value in the matrix, set it into the game grid
   return game;
}

// Free everything / set everything to 0
void bp_destroy(BPGame* b) {
   // BELOW DOESNT MATTER IF FREEING B
   // b->ncols = 0; 
   // b->nrows = 0; 
   // b->gridHistorySize = 0;
   free(b->gridHistory);
   free(b->grid);
   free(b);
}

// Displays everything 
void bp_display(BPGame* b) {}

// pop balloon 
int bp_pop(BPGame * b, int r, int c) {return 0;}

// checks if any more floats can be made
int bp_is_compact(BPGame * b) {return 0;}

// floats 
void bp_float_one_step(BPGame * b) {}

// returns the score
int bp_score(BPGame * b) {
   return b->score;
}

// checks to see if the character at that index is a balloon 
int bp_get_balloon(BPGame * b, int r, int c) {return 0;}

// checks to see if any more clusters exist (which means that they are poppable)
int bp_can_pop(BPGame * b) {return 0;}

// undos 
