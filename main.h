#ifndef MAIN_H
#define MAIN_H

#include "gba.h"

// TODO: Create any necessary structs

/*
* For example, for a Snake game, one could be:
*
* struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* };
*
* Example of a struct to hold state machine data:
*
* struct state {
*   int currentState;
*   int nextState;
* };
*
*/
// top left corner coordinates
// size of plane is 30x30
typedef struct hero {
	int row;
	int col;
} Hero;

void heroRun(Hero *b, int row, int col);
int collisionCheck(int cur_row, int cur_col, int target_row_s, int target_col_s, int target_row, int target_col);
#endif
