// file:  bpgame.c

/** #include statements... **/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bpgame.h"
/** TYPEDEFS / STRUCTs HERE ***/
struct bpgame { 
  int nrows; // number of rows
  int ncols; // number of columns
  int score; // the current score of the person 

  // char** grid; // the grid itself 
  // int gridHistorySize; // the size of the grid history array
  // int gridHistoryIndex; // the location of the grid history index
  // int gridHistoryMaxIndex; // the highest location the index has been at
  // char*** gridHistory; // the history of the grid
};

// struct gridHistory {
//   char*** cube; 
//   int size; 
//   int index; 
//   int maxIndex; 
// }

/** Helper functions  */
// This is here so srand() is only called once
int isRandInit = 0;

// Sets the seed if it hasnt been set before
void randInit() {
  if(!isRandInit) {
    isRandInit = 1;
    srand((unsigned) time(0));
  }
}

// creates a matrix using two malloc()'s
char** createGrid(int nrows, int ncols) {
  char* bigline = malloc(sizeof(char) * ncols * nrows); // one big line of stuff
  char** grid = malloc(sizeof(char*) * nrows); // points to particular places in that line 
  for(int r = 0; r < nrows; r++) // for each row
    grid[r] = &bigline[r * nrows]; // set each row to a part of the big line 
  return grid; 
}

// This pushes the current grid onto the history stack 
// void gridHistoryPush(BPGame * game) { // CURRENTLY NOT WORKING BTW
//   if(game->gridHistoryIndex >= game->gridHistorySize) { // resize
//     char*** temp = game->gridHistory;
//     game->gridHistory = calloc(game->gridHistorySize * 2, sizeof(char**));
    
//     int i = 0;
//     for(i; i < game->gridHistorySize; i++) // copy over each grid 
//       game->gridHistory[i] = temp[i];

//     game->gridHistorySize *= 2;
//     free(temp); // free previous pointer 

//     for(i; i < game->gridHistorySize; i++)
//       game->gridHistory[i] = createGrid(game->nrows, game->ncols); // create a grid for the rest of the spots
//   }

//   for(int r = 0; r < game->nrows; r++)
//     for(int c = 0; c < game->ncols; c++)
//       game->gridHistory[game->gridHistoryIndex][r][c] = game->grid[r][c]; // copy EVERY INDEX, cannot set pointers equal 
//   game->gridHistoryIndex++; // incremenet marker

//   if(game->gridHistoryMaxIndex < game->gridHistoryIndex)
//     game->gridHistoryMaxIndex = game->gridHistoryIndex;
// }

// This returns the grid at the current index, and subtracts it by one. CAN RETURN NULL 
// char** gridHistoryPop(BPGame * game) {
//   if(game->gridHistoryIndex <= 0) {
//     return NULL;
//   }
//   game->gridHistoryIndex -= 1;
//   return game->gridHistory[game->gridHistoryIndex]; 
// }

// void gridUndoFromHistory(BPGame* game) {
//   char** history = gridHistoryPop(game);
//   printf("some char maybe? %c\n", history[4][0]);
//   game->grid = history; 
//   // for(int i = 0; i < game->nrows; i++)
//   //     game->grid[i] = history[i]; // all you need to do is copy the rows, no need for anything else since it passes the pointer 
//     // for(int j = 0; j < game->ncols; j++)
//     //   game->grid[i][j] = history[i][j];
// }

/*** IMPLEMENTATION OF bp_XXXX FUNCTIONS HERE  ****/

// gets initial value, NOTE: you still need to init grid
BPGame* bp_init(int nrows, int ncols) {
  if(nrows > MAX_ROWS || ncols > MAX_COLS) { // If the rows or columns are greater than 40, error out
    fprintf(stderr, "Error with nrows (%i) or ncols (%i) in BPGame creation!\n", nrows, ncols);
    return NULL;
  }
  BPGame* game = malloc(sizeof(BPGame)); // allocate space for the game 
  game->nrows = nrows; // sets the rows
  game->ncols = ncols; // sets the cols
  game->grid = malloc(sizeof(char*) * game->nrows);
  // game->gridHistorySize = 5; // set the history size
  // game->gridHistory = malloc(sizeof(char**) * game->gridHistorySize); // creates the history
  // game->gridHistoryIndex = 0;
  // game->gridHistoryMaxIndex = 0;

  // for(int i = 0; i < game->gridHistorySize; i++)
  //   game->gridHistory[i] = createGrid(game->nrows, game->ncols);

  return game;
}

BPGame* bp_create(int nrows, int ncols) {
  randInit();
  BPGame* game = bp_init(nrows, ncols); // gets init of game
  char* grid = malloc(sizeof(char) * game->ncols * game->nrows); // create one big line 
  for(int i = 0; i < game->nrows * game->ncols; i++) { // set eaach value to a random "color"
    switch(rand() % 4) { // gets a random number, and sets that point depending on that random number
      case 0:
        grid[i] = Red;
        break;
      case 1: 
        grid[i] = Blue;
        break;
      case 2: 
        grid[i] = Green;
        break;
      case 3:
        grid[i] = Yellow;
        break;
    }
  }

  for(int r = 0; r < game->nrows; r++) { // for each row
    game->grid[r] = &grid[r * game->nrows]; // set it to the the corresponding place in grid var  
  }

  // gridHistoryPush(game);
  return game; 
}

// the same thing as above, but getting vlaues from a matrix instead of random
BPGame* bp_create_from_mtx(char mtx[][MAX_COLS], int nrows, int ncols) {
  BPGame* game = bp_init(nrows, ncols); // gets init of game
  char* grid = malloc(sizeof(char) * ncols * nrows); // create one big line 

  for(int r = 0; r < nrows; r++) // for each row
    game->grid[r] = &grid[r * nrows]; // set it to the the corresponding place in grid var   

  for(int i = 0; i < nrows; i++) {
    for(int j = 0; j < ncols; j++) {
      if(mtx[i][j] && ((mtx[i][j] == Red) || (mtx[i][j] == Blue) || (mtx[i][j] == Green) || (mtx[i][j] == Yellow))) {
        game->grid[i][j] = mtx[i][j]; // for each value in the matrix, set it into the game grid
      } else {
        fprintf(stderr, "Error! matrix color is invalid in bp_create_from_mtx!\n");
        return NULL;
      }
    }
  }

  // gridHistoryPush(game);
  return game;
}

// Free everything / set everything to 0
void bp_destroy(BPGame* b) {
  // BELOW DOESNT MATTER IF FREEING B
  // b->ncols = 0; 
  // b->nrows = 0; 
  // b->gridHistorySize = 0;
  
  // for(int s = 0; s < b->gridHistoryMaxIndex + 1; s++) {
  //   // free(b->gridHistory[s][0]); // free the big row
  //   // free(b->gridHistory[s]); // free the pointers 
  // }
  // free(b->gridHistory); // the history itself 

  free(b->grid[0]); // remember grid column is one big malloc
  free(b->grid); // remember grid row is one big malloc
  free(b);
}

// Display util functions
// repeats chars (used for repeating spaces)
void repeatChar(char c, int times) {
  while(times--)
    printf("%c", c);
}
// displays the column numbers (as seen in .h file)
void displayColumnNumbers(int number) {
  repeatChar(' ', 4 + 3); // add the spacers
  for(int i = 0; i < number; i++) {
    printf("%i ", i / 10); // print out the tens place
  }
  printf("\n");
  repeatChar(' ', 4 + 3); // add the spacers
  for(int i = 0; i < number; i++) {
    printf("%i ", i % 10); // print out the ones place
  }
  printf("\n");
}

// Displays everything 
void displayLine(int numCols){
  repeatChar(' ', 4 + 1);
  printf("+");
  repeatChar('-', (numCols * 2) + 1);
  printf("+\n");
}
void bp_display(BPGame* b) {
  displayLine(b->ncols);
   
  for (int r = 0; r < b->nrows; r++){
    repeatChar(' ', 2);
    printf("%d%d |", r / 10, r % 10);
    for (int c = 0; c < b->ncols; c++){
      printf(" %c", b->grid[r][c]);
    }
    printf(" |\n");
  }
   
  displayLine(b->ncols);   
  displayColumnNumbers(b->ncols);
}

// pop balloon 
int bp_pop(BPGame * b, int r, int c) {
  if(r < 0 || r >= b->nrows || c < 0 || c >= b->ncols)
    return 0;
   
  int num_pops = 0;
  int type = bp_get_balloon(b, r, c);

  if (type > 0) {
    b->grid[r][c] = None;
    num_pops += 1;

    if(bp_get_balloon(b, r + 1, c) == type)
      num_pops += bp_pop(b, r + 1, c);
    if(bp_get_balloon(b, r - 1, c) == type)
      num_pops += bp_pop(b, r - 1, c);
    if(bp_get_balloon(b, r, c + 1) == type)
      num_pops += bp_pop(b, r, c + 1);
    if(bp_get_balloon(b, r, c - 1) == type)
      num_pops += bp_pop(b, r, c - 1);
  }

  return num_pops;
}

// // checks if any more floats can be made
int bp_is_compact(BPGame * b) {
  for(int j = 0; j < b->ncols; j++) // left to right
    for(int i = b->nrows - 1; i > 0; i--) // bottom to up, dont need to check final row since nothing above it 
      if(bp_get_balloon(b, i, j) > 0) // check to see if index is balloon 
        for(int k = i - 1; k > -1; k--) // traverse upwards
          if(bp_get_balloon(b, k, j) < 0) // check to see if it is a balloon  
            return 0; // if air, then return 0
          else
            break; // everthing above is a balloon, so go to next column 
  return 1; // else return 1
}

// floats by one step
void bp_float_one_step(BPGame * b) {
  if(bp_is_compact(b))
    return; 

  for(int i = 0; i < b->nrows - 1; i++)
    for(int j = 0; j < b->ncols; j++) {
      if(bp_get_balloon(b, i, j) == -1) {
        // printf("loc: [%i][%i] => %c\n", i, j, b->grid[i][j]);
        char temp = b->grid[i][j];
        b->grid[i][j] = b->grid[i + 1][j];
        b->grid[i + 1][j] = temp; 
      }
    }
}

// // returns the score
int bp_score(BPGame * b) {
  return b->score;
}

// // checks to see if the character at that index is a balloon 
int bp_get_balloon(BPGame * b, int r, int c) {
  if(r < 0 || r >= b->nrows || c < 0 || c >= b->ncols)
    return -2;

  char balloon = b->grid[r][c];
  switch(balloon){
    case None: 
      return -1;
      break;
    case Red: 
      // printf("Red!\n");
      return 1;
      break;
    case Blue: 
      // printf("Blue!\n");
      return 2; 
      break;
    case Green: 
      // printf("Green!\n");
      return 3;
      break;
    case Yellow: 
      // printf("Yellow!\n");
      return 4; 
      break;
    default: 
      printf("this shouldnt be here! %c at [%i][%i]\n", balloon, r, c);
      return -1;
      break;
  }
  return -1;
}

// // checks to see if any more clusters exist (which means that they are poppable)
int bp_can_pop(BPGame * b) {
  int type = -1;
  for(int i = 0; i < b->nrows; i++)
    for(int j = 0; j < b->ncols; j++) {
      type = b->grid[i][j];
      if(type > 0 && (bp_get_balloon(b, i + 1, j) == type || bp_get_balloon(b, i - 1, j) == type || bp_get_balloon(b, i, j + 1) == type || bp_get_balloon(b, i, j - 1) == type))
        return 1;
    }
  return 0;
}

// // undos 
// int bp_undo(BPGame * b) {return 0;}

int main() {
  randInit();
  // int rows = 5;
  // int cols = 5;
  // BPGame* game = bp_create(rows, cols);

  char arr[40][40] = {
    {'^', '^', '+', '+', '^'}, 
    {'=', '^', '+', 'o', '^'}, 
    {'=', '^', '^', 'o', '^'}, 
    {'=', '=', '^', 'o', '^'}, 
    {'=', '=', '=', '=', '='}
  };

  BPGame* game = bp_create_from_mtx(arr, 5, 5);
  bp_display(game);

  bp_pop(game, 2, 2);
  while(!bp_is_compact(game)) {
    bp_float_one_step(game);
    bp_display(game);
  }

  gridUndoFromHistory(game);
  bp_display(game);
  // bp_display(game);
  // int num;
  // num = bp_pop(game, 0, 4);
  // printf("num: %i\n", num);
  // while(!bp_is_compact(game)) {
  //   bp_float_one_step(game);
  //   bp_display(game);
  // }

  bp_destroy(game);
  return 0;
}

/* 
  Current: 
    All the grid functions are working (except you can still pop a single balloon, ik the fix)
    The history is working for the most part
    There is someting that I need to do with the frees 
    Then I need to check if the grow works fine 
    Then test out undo 
    Then the rest on the TODO

  TODO:
  [x] Figure out what is going wrong wtih 2d array in create from mtx
  [] Get history working / undo 
  [] Test out the functions 
  [] Remove main & Connect to bpop.c and test it 
  [] Submit
*/