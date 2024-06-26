#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t *state, unsigned int snum);
static char next_square(game_state_t *state, unsigned int snum);
static void update_tail(game_state_t *state, unsigned int snum);
static void update_head(game_state_t *state, unsigned int snum);

/* Task 1 */
game_state_t *create_default_state() {
  //Make game_state_t struct instance 
  //[has #rows, 
  //#snakes, 
  //char ** board, 
  //and snake_t * snakes]
  
  game_state_t * default_board;
  
  default_board = malloc(sizeof(game_state_t));
  
  default_board->num_rows = 18;
  
  default_board->num_snakes = 1;


  //making the board which is an array of strings
  default_board-> board = malloc(default_board->num_rows * sizeof(char *)); //allocate memory
                             //HARDCODE THE BOARD //board = malloc(sizeof(inner thing));
  //Allocate memory for each row:
  for (int i = 0; i < default_board->num_rows; i++) {
    default_board->board[i] = malloc(21 * sizeof(char)); //20 columns + 1 for null terminator!
  }
  //Hardcoded layout:
  const char* initial_board[] = {
      //EACH ROW
      //
      "####################",
      "#                  #",
      "# d>D    *         #",
      "#                  #",
      "#                  #",
      "#                  #",
      "#                  #",
      "#                  #",
      "#                  #",
      "#                  #",
      "#                  #",
      "#                  #",
      "#                  #",
      "#                  #",
      "#                  #",
      "#                  #",
      "#                  #",
      "####################"
  };


          
  //fill board w initial_board hardcoded template

  for (int i = 0; i < default_board->num_rows; i++) {
      strcpy(default_board->board[i], initial_board[i]); //strcpy(dest, src)
  }

  //make snakes and all; initiailze the snake
  default_board->snakes = malloc(default_board->num_snakes * sizeof(snake_t)); //makes snake w room for 1 snake
                                                                    
  //5 attributes to fill in!!!
  default_board->snakes[0].tail_row = 2;
  default_board->snakes[0].tail_col = 2;
  default_board->snakes[0].head_row = 2;
  default_board->snakes[0].head_col = 4;
  default_board->snakes[0].live = true;

  return default_board;
  //note: eventually have to free default_board
  //"strcpy may be helpful" 


    
/*

#define ROWS 18
#define COLS 20

  char* bard[ROWS];
  for (int i = 0; i < ROWS; i++) {
      bard[i] = malloc((COLS + 1) * sizeOf(char));
      for (int j = 0; j < COLS; j++) {
          if (i == 0 | i == ROWS - 1 | j == 0 | j == COLS - 1) *(bard[i] + j) = '#';
          else *(bard[i] + j) = ' ';
      }
      *(bard[i] + j) = '\0';
  }
  // Make og snake
  // Tail
  *(bard[2] + 2) = 'd';
  // Mid
  *(bard[2] + 3) = '>';
  // Head
  *(bard[2] + 4) = 'D';
  // Make first fruit
  *(bard[2] + 9) = "*";
  return bard;
  */
}

/* Task 2 */
void free_state(game_state_t *state) {
  // SHOULD WE CHECK IF THEY EXIST??
  //free each string on rows
   for (int i = 0; i< state->num_rows; i++) {
       free(state->board[i]);
   }
   //free board array attribute
   free(state->board);
   //free snake array attribute
   free(state->snakes);
   //free state pointer
   free(state);
   return;
}

/* Task 3 */
void print_board(game_state_t *state, FILE *fp) {
  
  for (int i = 0; i < state-> num_rows; i++) {
      fprintf(fp, "%s\n", state->board[i]);
  }
  return;
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t *state, char *filename) {
  FILE *f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t *state, unsigned int row, unsigned int col) { return state->board[row][col]; }

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch) {
  state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  if ((c == 'w') | (c == 'a') | (c == 's') | (c == 'd')) return true;
  else return false;
 }
/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
    if ((c == 'W') | (c == 'A') | (c == 'S') | (c == 'D') | (c == 'x')) return true;
    return false;
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c) {
    if (is_tail(c) | is_head(c) | (c == '^') | (c == '<') | (c == 'v') | (c == '>')) return true;
    return false;
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  switch (c) {
      case ('^'):
          return 'w';
      case ('<'):
          return 'a';
      case ('v'):
          return 's';
      case ('>'):
          return 'd';   
      default: return c;
  }
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c) {
  switch (c) {
      case ('W'):
          return '^';
      case ('A'):
          return '<';
      case ('S'):
          return 'v';
      case ('D'):
          return '>';
      default: return c;
  }
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
  switch (c) {
      case ('v' | 's' | 'S'):
          return cur_row + 1;
      case ('^' | 'w' | 'W'):
          return cur_row - 1;
      default:
          return cur_row;
  }
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
  switch (c) {
      case ('>' | 'd' | 'D'):
          return cur_col + 1;
      case ('<' | 'a' | 'A'):
          return cur_col - 1;
      default:
          return cur_col;
  }
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t *state, unsigned int snum) {
  // TODO: Implement this function.
  return '?';
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t *state, unsigned int snum) {
  // TODO: Implement this function.
  return;
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t *state, unsigned int snum) {
  // TODO: Implement this function.
  return;
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state)) {
  // TODO: Implement this function.
  return;
}

/* Task 5.1 */
char *read_line(FILE *fp) {
  // TODO: Implement this function.
  return NULL;
}

/* Task 5.2 */
game_state_t *load_board(FILE *fp) {
  // TODO: Implement this function.
  return NULL;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t *state, unsigned int snum) {
  // TODO: Implement this function.
  return;
}

/* Task 6.2 */
game_state_t *initialize_snakes(game_state_t *state) {
  // TODO: Implement this function.
  return NULL;
}
