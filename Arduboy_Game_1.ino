#include <ArduboyTonesPitches.h>
#include <ArduboyTones.h>

#include <Arduboy2.h>
#include "images.h"
#include "tones.h"
#include <EEPROM.h>
#include "eeprom.h"


Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);

int gamestate = 0;
int carx = 5;
int cary = 54;
int carsize = 9;
int enemyy = 0;
int enemyx = 123;
int enemysize = 5;
int enemyright = -1;
bool cardirection = false;
int enemyX = 0;
int enemyY = 0;
int enemySize = 5;
int enemyX2 = 0;
int enemyY2 = 0;
int enemySize2 = 5;
int enemyDown2 = 1;
int enemyRight2 = 1;
int enemydown = 1;
uint8_t score = 0;
int coinX = 63;
int coinY = 32;
int coinwidth = 4;
int coinheight = 5;
int highscore = 0;

#define GAME_TITLE 0
#define GAME_PLAY 1
#define GAME_OVER 2

void titlescreen() {
  arduboy.setCursor(0, 0);
  arduboy.print("Coin Rush!");
  arduboy.print("\n");
  arduboy.print(highscore);

  if (score > highscore) {
    highscore = score;
  }
  
  if (arduboy.justPressed(A_BUTTON)) {
    gamestate = GAME_PLAY;
    enemyy = random(11, 54);
    enemyX = random(11, 113);
    enemyX2 = random(0, 119);
    enemyY2 = random(0, 63);
    carx = 5;
    cary = 54; 
    score = 0;
   
  }

  if (arduboy.justPressed(B_BUTTON)) {
    EEPROM.put(EEPROM_STORAGE_SPACE_START, highscore);
  }

}

void gameplay() {
  
  arduboy.setCursor(0, 0);
  arduboy.print(score);

  if (cardirection == false) {
    Sprites::drawOverwrite(carx, cary, car, 0);
  }

  if (cardirection == true) {
    Sprites::drawOverwrite(carx, cary, car, 1);
  }
  
    if (arduboy.pressed(DOWN_BUTTON)) {
      cary += 1;
      Sprites::drawOverwrite(carx, cary, car, 1);
      cardirection = true;
      }
    

    if (arduboy.pressed(UP_BUTTON)) {
      cary -= 1;
      Sprites::drawOverwrite(carx, cary, car, 1);
      cardirection = true;
    }

    if (arduboy.pressed(RIGHT_BUTTON)) {
      carx += 1;
      Sprites::drawOverwrite(carx, cary, car, 0);
      cardirection = false;
    }

    if (arduboy.pressed(LEFT_BUTTON)) {
      carx -= 1;
      Sprites::drawOverwrite(carx, cary, car, 0);
      cardirection = false;
    }
  
  Sprites::drawOverwrite (coinX, coinY, coin, 0);

  arduboy.fillRect (enemyx, enemyy, enemysize, enemysize, WHITE);

  arduboy.fillRect (enemyX, enemyY, enemySize, enemySize, WHITE);

  arduboy.fillRect (enemyX2, enemyY2, enemySize2, enemySize2, WHITE);
  
  Rect enemy (enemyx, enemyy, enemysize, enemysize);

  Rect enemY (enemyX, enemyY, enemySize, enemySize);

  Rect enemY2 (enemyX2, enemyY2, enemySize2, enemySize2);
  
  Rect player (carx, cary, carsize, carsize);

  Rect coin (coinX, coinY, coinwidth, coinheight);
  
  if (arduboy.collide (player, enemy)) {
    gamestate = GAME_OVER;
    sound.tones(hitsound);
  }

  if (arduboy.collide (player, enemY)) {
    gamestate = GAME_OVER;
    sound.tones(hitsound);
  }

  if (arduboy.collide(player, enemY2)) {
    gamestate = GAME_OVER;
    sound.tones(hitsound);
  }

  if (arduboy.collide(player, coin)) {
    score = score + 1;
    coinX = random(0, 124);
    coinY = random(0, 59);
    sound.tones(coinsound);
  }

  if (enemyright == 1) {
    enemyx = enemyx + 1;
  }

  if (enemyright == -1) {
    enemyx = enemyx - 1;
  }

  if (enemyx == 123) {
    enemyright = -1;
  }

  if (enemyx == 0) {
    enemyright = 1;
  }

  if (enemydown == 1) {
    enemyY = enemyY + 1;
  }

  if (enemydown == -1) {
    enemyY = enemyY - 1;
  }
  
  if (enemyY == 59) {
    enemydown = - 1;
  }

  if (enemyY == 0) {
    enemydown = 1;
  }

  if (enemyDown2 == 1) {
    enemyY2 = enemyY2 + 1;
  }

  if (enemyDown2 == -1) {
    enemyY2 = enemyY2 - 1;
  }

  if (enemyRight2 == 1) {
    enemyX2 = enemyX2 + 1;
  }

  if (enemyRight2 == -1) {
    enemyX2 = enemyX2 - 1;
  }

  if (enemyY2 == 0) {
    enemyDown2 = 1;
  }

  if (enemyY2 + enemySize2 == 63) {
    enemyDown2 = - 1;
  }

  if (enemyX2 == 0) {
    enemyRight2 = 1;
  }

  if (enemyX2 + enemySize2 == 123) {
    enemyRight2 = - 1;
  }

}

void gameoverscreen() {
  arduboy.setCursor(0, 0);
  arduboy.print("game over screen");
  
  if (arduboy.justPressed(A_BUTTON)) {
    gamestate = GAME_TITLE;
  }
}

void gameloop() {

  switch(gamestate) {
    
    case GAME_TITLE:
      titlescreen();
      break;

    case GAME_PLAY:
      gameplay();
      break;

    case GAME_OVER:
      gameoverscreen();
      break;
      
  }
}

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(45);
  arduboy.initRandomSeed();
  arduboy.clear();

}

void loop() {
  if (!arduboy.nextFrame()) {
    return;
  }
  
  arduboy.clear();
  arduboy.pollButtons();
  gameloop();
  arduboy.display();

}
