// file:  bpgame.c

/** #include statements... **/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bpgame.h"

/** TYPEDEFS / STRUCTs HERE ***/
struct bpgame {
   // YOU DECIDE WHAT TO PUT IN HERE TO CAPTURE THE STATE OF
   //   A "RUN" OF THE GAME.....
   int nrows; // number of rows
   int ncols; // number of columns
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

BPGame* bp_create(int nrows, int ncols) {
   if(nrows > MAX_ROWS || ncols > MAX_COLS) { // If the rows or columns are greater than 40, error out
      fprintf(stderr, "%s", "Error with nrows (%i) or ncols (%i) in bp_create!\n", nrows, ncols);
      return NULL;
   }
   randInit();

   BPGame* game = malloc(sizeof(BPGame)); // allocate space for the game 
   game->nrows = nrows; // sets the rows
   game->ncols = ncols; // sets the cols
   game->gridHistorySize = 5; // set the history size
   game->gridHistory = malloc(sizeof(char) * game->ncols * game->nrows * game->gridHistorySize); // creates the history
   
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
   if(nrows > MAX_ROWS || ncols > MAX_COLS) { // If the rows or columns are greater than 40, error out
      fprintf(stderr, "%s", "Error with nrows (%i) or ncols (%i) in bp_create!\n", nrows, ncols);
      return NULL;
   }

   BPGame* game = malloc(sizeof(BPGame)); // allocate space for the game 
   game->nrows = nrows; // sets the rows
   game->ncols = ncols; // sets the cols
   game->gridHistorySize = 5; // sets the history size
   game->gridHistory = malloc(sizeof(char) * game->ncols * game->nrows * game->gridHistorySize); // creates the history buffer

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
   b->ncols = 0;
   b->nrows = 0;
   b->gridHistorySize = 0;
   free(b->gridHistory);
   free(b->grid);
   free(b);
}