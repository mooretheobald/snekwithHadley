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
    default_board->board[i] = malloc(22 * sizeof(char)); //20 columns + 1 for new line + 1 for null terminator!
  }
  //Hardcoded layout:
  const char* initial_board[] = {
      //EACH ROW
      //
      "####################\n",
      "#                  #\n",
      "# d>D    *         #\n",
      "#                  #\n",
      "#                  #\n",
      "#                  #\n",
      "#                  #\n",
      "#                  #\n",
      "#                  #\n",
      "#                  #\n",
      "#                  #\n",
      "#                  #\n",
      "#                  #\n",
      "#                  #\n",
      "#                  #\n",
      "#                  #\n",
      "#                  #\n",
      "####################\n"
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
      {
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
      fprintf(fp, "%s", state->board[i]);
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
        case 'v':
            return cur_row + 1;
        case 's':
            return cur_row + 1;
        case 'S':
            return cur_row + 1;
        case '^':
            return cur_row - 1;
        case 'w':
            return cur_row - 1;
        case 'W':
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
      case '>':
          return cur_col + 1;
      case 'd':
          return cur_col + 1;
      case 'D':
          return cur_col + 1;
      case '<':
          return cur_col - 1;
      case 'a':
          return cur_col - 1;
      case 'A':
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
  


  // original
  //
  /*
  snake_t temp = state->snakes[snum];
  unsigned int temprow = temp.head_row;
  unsigned int tempcol = temp.head_col;
  char tempchar = get_board_at(state, temprow, tempcol);
  return get_board_at(state, get_next_row(temprow, tempchar), get_next_col(tempcol, tempchar));

    */
  //other
  snake_t curr_s = state->snakes[snum];
  char s = get_board_at(state, curr_s.head_row,curr_s.head_col);
  //get next stuff from head
  unsigned int next_row = get_next_row(curr_s.head_row,s);
  unsigned int next_col = get_next_col(curr_s.head_col,s);
  //return board at this new spot
  return get_board_at(state, next_row, next_col);

}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t *state, unsigned int snum) {

  snake_t curr_s = state->snakes[snum];
  char prev_char = get_board_at(state, curr_s.head_row, curr_s.head_col);
  // update head on game board; add a character where the snake is moving and replace prev_head with an arrow "^v<>" ONLY TEXT, no attributes
  set_board_at(state, curr_s.head_row, curr_s.head_col, head_to_body(prev_char));
  unsigned int new_row = get_next_row(curr_s.head_row, prev_char);
  unsigned int new_col = get_next_col(curr_s.head_col, prev_char);
  set_board_at(state, new_row, new_col, prev_char);
  //state[curr_s.head_row][curr_s.head_col] = ; //turnednto PREVIOUS char
  // update head in the snake_t struct, w head_row and head_col ONLY ATTRIBUTES, no text
  state->snakes[snum].head_row = new_row;
  state->snakes[snum].head_col = new_col;
  
  return;
}

/*
  Task 4.4

  Helper function for update_state. Update theail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the 
*/

  // replace next body segment with new tail spot
static void update_tail(game_state_t *state, unsigned int snum) {
  snake_t curr_s = state->snakes[snum];
  char prev_char = get_board_at(state, curr_s.tail_row, curr_s.tail_col);
  // reset old tail to space
  // state[curr_s.head_row][curr_s.head_col] = head_to_body(prev_char);
  unsigned int new_row = get_next_row(curr_s.tail_row, prev_char);
  unsigned int new_col = get_next_col(curr_s.tail_col, prev_char);
  set_board_at(state, new_row, new_col, body_to_tail(get_board_at(state, new_row, new_col)));
  set_board_at(state, curr_s.tail_row, curr_s.tail_col, ' ');
  //state[curr_s.head_row][curr_s.head_col] = ; //turnednto PREVIOUS char
  // update tail in snake structure
  state->snakes[snum].tail_row = new_row;
  state->snakes[snum].tail_col = new_col;
  
  return;
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state)) {
     // Iterate over each snake
    for (unsigned int snum = 0; snum < state->num_snakes; snum++) {
        if (state->snakes[snum].live) {
            // Determine next position
           // unsigned int next_row = get_next_row(state->snakes[snum].head_row, state->snakes[snum].head_direction);
            //unsigned int next_col = get_next_col(state->snakes[snum].head_col, state->snakes[snum].head_direction);
            
            // Get the character in the next square
            char next_char = next_square(state, snum);
            
            // Check for collisions with walls or other snakes
            if (next_char == '#' || is_snake(next_char)) {
                // Snake dies
                state->snakes[snum].live = false;
                // Place an 'x' at the head's current position
                set_board_at(state, state->snakes[snum].head_row, state->snakes[snum].head_col, 'x');
            } else if (next_char== '*') { // Next char is fruit
                // Snake eats fruit
                add_food(state); // Add new fruit
                //state->snakes[snum].length++; // Increment length of the snake
                update_head(state, snum); // Move head to the next position
                                          // //TAIL NOT MOVED
            } else {
                // Move the snake
                update_head(state, snum); // Move head to the next position
                update_tail(state, snum); // Move tail to maintain length
            }
        }
    }
}





/* Task 5.1 */
char *read_line(FILE *fp) {
    //make temp
    char temp[100];
    char *ptr = fgets(temp, sizeof(temp),fp);
    if (!ptr) return NULL;
    long unsigned int length = strlen(temp);
    char* retval = malloc((length + 1) * sizeof(char));
    if (!retval) return NULL;
    strcpy(retval, temp);
    return retval;



    /* ORIGINAL
    char* t1 = malloc(500);
    if (!t1) return NULL;
    fgets(t1, 500, fp);
    size_t cols = strlen(t1);
    char* t2 = realloc(t1, (cols + 1) * sizeof(char) + 1);
    return t2;

    
    char* retval = malloc(sizeof(char*));
    if (!retval) return NULL;
    retval = fgets(retval, 500, fp);
    char* temp = malloc(sizeof(char*));
    if (!temp) return NULL;
    strcpy(temp, retval);
    free(retval);
    return temp;
    */
}




/* Task 5.2 */
game_state_t *load_board(FILE *fp) {

    game_state_t* loaded = malloc(sizeof(game_state_t)); //NOT sizeof loaded
    if (!loaded) return NULL;

    //copy board to temp to get numrows
    char* temp[100];
    char* curr_line;
    unsigned int row = 0;
    while ((curr_line = read_line(fp))) {
        temp[row] = curr_line;
        row++;
    }
    loaded->num_rows = row;

    //add snakes as per instructions
    loaded->num_snakes = 0;
    loaded->snakes = NULL;

    //malloc the board
    loaded->board = malloc(loaded->num_rows * sizeof(char*));
    if (!loaded->board) {
        free(loaded);
        return NULL;
    }
    //copy board to loaded; WHEN DONE, FREE
    for (unsigned int i = 0; i< loaded->num_rows; i++) {
        long unsigned int len = strlen(temp[i]);
        char* curr_row = malloc((len + 1) * sizeof(char));
        if (!curr_row) {
            for (unsigned int j = 0; j < i; j++) {
                free(loaded->board[j]);
            }
            free(loaded->board);
            free(loaded);
            return NULL;
        }
        strcpy(curr_row, temp[i]);
        loaded->board[i] = curr_row;
    }
    return loaded;

  }
    /* ORIGINAL
    game_state_t* loaded = malloc(sizeof(loaded));
    if (!loaded) return NULL;
    loaded->num_rows = 0;
    char** temp = malloc(1000 * sizeof(char*));
    while (1) {
        if (feof(fp)) break;
        loaded->num_rows++;
        temp[loaded->num_rows - 1] = malloc(sizeof(char*));
        if (!temp[loaded->num_rows - 1]) return NULL;
        temp[loaded->num_rows - 1] = read_line(fp);
    }
    size_t cols = strlen(temp[0]);
    free(temp[loaded->num_rows - 1]);
    loaded->num_rows--;
    loaded->board = malloc(loaded->num_rows * sizeof(char*));
    if (!loaded->board) return NULL;
    for (int i = 0; i < loaded->num_rows; i++) {
        loaded->board[i] = malloc((cols + 1) * sizeof(char));
        if (!loaded->board[i]) return NULL;
        strcpy(loaded->board[i], temp[i]);
        free(temp[i]);
    }
    free(temp);
    return loaded;
    */






/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t *state, unsigned int snum) {
    snake_t curr_s = state->snakes[snum];
    unsigned int curr_row = curr_s.tail_row;
    unsigned int curr_col = curr_s.tail_col;

    char curr_char = state->board[curr_row][curr_col];

    while (!is_head(curr_char) && is_snake(curr_char)) { //ADDED is_snake
        curr_row = get_next_row(curr_row, curr_char);
        curr_col = get_next_col(curr_col, curr_char);
        curr_char = state->board[curr_row][curr_col];
    }
    //to change must go into snakes, NOT just update curr_s......
    state->snakes[snum].head_row = curr_row;
    state->snakes[snum].head_col = curr_col;
    return;
}

/* Task 6.2 */
game_state_t *initialize_snakes(game_state_t *state) {
    unsigned int numsnakes = 0;
    snake_t temp_snakes[100];
    //iterate through the whole board
    for (unsigned int row = 0; row < state->num_rows; row++) {
        for (unsigned int col = 0; col < strlen(state->board[row]); col ++) {
            char next_char = get_board_at(state, row, col);
            if (is_tail(next_char)) {
                //if there is a tail, fill in temp w the snake info
                //snake_t curr_s = temp_snakes[numsnakes];
                temp_snakes[numsnakes].live = true;
                temp_snakes[numsnakes].tail_row = row;
                temp_snakes[numsnakes].tail_col = col;
                numsnakes++;
            }
        }
    }

    //set up ret board snakes array

    state->num_snakes = numsnakes;
    state->snakes = malloc(state->num_snakes * sizeof(snake_t));

    //fill in the state from the temp snakes array

   for (unsigned int s = 0; s < state->num_snakes; s++) {
    state->snakes[s] = temp_snakes[s];
    //calling find head finds and sets the head
    find_head(state, s);
  }
  return state;
}


