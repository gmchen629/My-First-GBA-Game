#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include "gba.h"
#include "images/start.h"
#include "images/win.h"
#include "images/loss.h"
#include "images/hero.h"
#include "images/monster.h"
#include "images/monster_small.h"

Hero b;
int npc_size = 35;

enum gba_state {
  START,
  INIT,
  PLAY,
  WIN,
  LOSE
};

int main(void) {
  REG_DISPCNT = MODE3 | BG2_ENABLE;

  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  // Load initial application state
  enum gba_state state = START;

  int timer = 0;
  int stop = 300;
  int pace = 2;
  int target_row = 90;
  int target_col = 150;
  int target_row_s = 45;
  int target_col_s = 75;
  int start_row = 20;
  int start_col = 20;
  char strbuffer[100];

  while (1) {
    currentButtons = BUTTONS; // Load the current state of the buttons
    switch (state) {

      case START:
        waitForVBlank();
        drawFullScreenImageDMA(start);
        drawString(45, 60, "HERO VS MONSTER", WHITE);
        drawString(70, 75, "ENTER TO PLAY", WHITE);
        if (KEY_DOWN(BUTTON_START, currentButtons)) {
          state = INIT;
        }
        if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
          state = START;
        }
        break;

      case INIT:
        waitForVBlank();
        fillScreenDMA(BLACK);
        timer = 0;
        Hero h;
        Hero *heroptr = &h; 
        heroptr->row = start_row;
        heroptr->col = start_col;
        drawImageDMA(start_row, start_col, npc_size, npc_size, hero);
        drawImageDMA(target_row, target_col, npc_size, npc_size, monster);
        drawImageDMA(target_row_s, target_col_s, npc_size, npc_size, monster_small);
        state = PLAY;
        break;

      case PLAY:
        waitForVBlank();
        drawRectDMA(5, 180, 70, 30, BLACK);
        drawImageDMA(heroptr->row, heroptr->col, npc_size, npc_size, hero);
        sprintf(strbuffer, "TIME: %d", timer / 60);
        drawString(5, 180, strbuffer, WHITE);
        
        if (KEY_DOWN(BUTTON_UP, currentButtons)) {
          heroRun(heroptr, -pace, 0);
        }
        if (KEY_DOWN(BUTTON_DOWN, currentButtons)) {
          heroRun(heroptr, pace, 0);
        }
        if (KEY_DOWN(BUTTON_RIGHT, currentButtons)) {
          heroRun(heroptr, 0, pace);
        }
        if (KEY_DOWN(BUTTON_LEFT, currentButtons)) {
          heroRun(heroptr, 0, -pace);
        }

        if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
          state = START;
        }

        int ifReach = 0;
        ifReach = collisionCheck(heroptr->row, heroptr->col, target_row_s, target_col_s, target_row, target_col);

        if (ifReach == 1) {
          fillScreenDMA(BLACK);
          drawImageDMA(heroptr->row, heroptr->col, npc_size, npc_size, hero);
          state = WIN;
        }

        if (ifReach == 2) {
          fillScreenDMA(BLACK);
          drawImageDMA(heroptr->row, heroptr->col, npc_size, npc_size, hero);
          state = LOSE;
        }

        if (timer == stop) {
          state = LOSE;
        }
        
        timer++;
        break;

      case WIN:
        waitForVBlank();
        drawFullScreenImageDMA(loss);
        waitForVBlank();
        drawString(50, 20, "CONGRATS!", WHITE);
        if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
          state = START;
        }
        break;

      case LOSE:
        waitForVBlank();
        drawFullScreenImageDMA(win);
        drawString(50, 20, "SORRY, YOU FAILED!", WHITE);
        if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
          state = START;
        }
        break;
    }

    waitForVBlank();

  }

  UNUSED(previousButtons); // You can remove this once previousButtons is used
  return 0;

}

void heroRun(Hero *heroptr, int row, int col) {
  drawRectDMA(heroptr->row, heroptr->col, npc_size, npc_size, BLACK);
  int next_row = heroptr->row + row;
  int next_col = heroptr->col + col;
  if (next_row >= 0 && next_row <= HEIGHT - npc_size) {
    heroptr->row = next_row;
  }
  if (next_col >= 0 && next_col <= WIDTH - npc_size) {
    heroptr->col = next_col;
  }
  drawImageDMA(heroptr->row, heroptr->col, npc_size, npc_size, hero);
}

int collisionCheck(int cur_row, int cur_col, int target_row_s, int target_col_s, int target_row, int target_col) {
  int ifReach = 0;
  if (cur_col >= target_col && cur_col <= target_col + npc_size && cur_row >= target_row && cur_row <= target_row + npc_size) {
    ifReach = 1;
  }

  if (cur_col >= target_col && cur_col <= target_col + npc_size && cur_row + npc_size >= target_row && cur_row + npc_size <= target_row + npc_size) {
    ifReach = 1;
  }
        
  if (cur_col + npc_size >= target_col && cur_col + npc_size <= target_col + npc_size && cur_row >= target_row && cur_row <= target_row + npc_size) {
    ifReach = 1;
  }

  if (cur_col + npc_size >= target_col && cur_col + npc_size <= target_col + npc_size && cur_row + npc_size >= target_row && cur_row + npc_size <= target_row + npc_size) {
    ifReach = 1;
  }

  if (cur_col >= target_col_s && cur_col <= target_col_s + npc_size && cur_row >= target_row_s && cur_row <= target_row_s + npc_size) {
    ifReach = 2;
  }

  if (cur_col >= target_col_s && cur_col <= target_col_s + npc_size && cur_row + npc_size >= target_row_s && cur_row + npc_size <= target_row_s + npc_size) {
    ifReach = 2;
  }
        
  if (cur_col + npc_size >= target_col_s && cur_col + npc_size <= target_col_s + npc_size && cur_row >= target_row_s && cur_row <= target_row_s + npc_size) {
    ifReach = 2;
  }

  if (cur_col + npc_size >= target_col_s && cur_col + npc_size <= target_col_s + npc_size && cur_row + npc_size >= target_row_s && cur_row + npc_size <= target_row_s + npc_size) {
    ifReach = 2;
  }

  return ifReach;

}
