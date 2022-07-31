#include <Arduboy2.h>
#include "images.h"

Arduboy2 arduboy;

int gamestate = 0;
int carx = 20;
int cary = 15;
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

// Target time for the delay timer
uint32_t delayTarget = 0;

#define GAME_TITLE 0
#define GAME_DELAY 4
#define GAME_PLAY 1
#define GAME_OVER 2
#define GAME_HIGH 3

void titlescreen() {
  if (arduboy.justPressed(A_BUTTON)) {
    enemyy = random(11, 54);
    enemyX = random(11, 113);

    // Set the target time to be the current time
    // plus 4000ms (i.e. 4 seconds)
    delayTarget = (millis() + (4 * 1000));

    // Change the state to the delay state
    gamestate = GAME_DELAY;
  }

  if (arduboy.justPressed(B_BUTTON)) {
    gamestate = GAME_HIGH;
  }
}

void drawPlayerCar()
{
  // This can be simplified further
  if (cardirection)
  {
    Sprites::drawOverwrite(carx, cary, car, 1);
  }
  else
  {
    Sprites::drawOverwrite(carx, cary, car, 0);
  }
}

// Does what it says on the tin
void drawEnemies()
{
  arduboy.fillRect(enemyx, enemyy, enemysize, enemysize, WHITE);
  arduboy.fillRect(enemyX, enemyY, enemySize, enemySize, WHITE);
  arduboy.fillRect(enemyX2, enemyY2, enemySize2, enemySize2, WHITE);
}

// Draws the cars and enemies
void drawGame()
{
  drawPlayerCar();
  drawEnemies();
}

void updateGameDelayState()
{
  // Get the current time
  uint32_t current = millis();

  // If the current time is greater than or equal to
  // the target time, then 4 seconds has elapsed
  if(current >= delayTarget)
  {
      // Change to the game play state to begin the game
      gamestate = GAME_PLAY;
  }

  // Calculate the number of seconds remaining
  int remainingSeconds = ((delayTarget - current) / 1000);

  // Print the number of seconds remaining
  arduboy.print(remainingSeconds);
  arduboy.println(F(" seconds remaining..."));

  drawGame();
}

void handlePlayerInput()
{
  if (arduboy.pressed(DOWN_BUTTON))
  {
    cary += 1;
    cardirection = true;
  }

  if (arduboy.pressed(UP_BUTTON))
  {
    cary -= 1;
    cardirection = true;
  }

  if (arduboy.pressed(RIGHT_BUTTON))
  {
    carx += 1;
    cardirection = false;
  }

  if (arduboy.pressed(LEFT_BUTTON))
  {
    carx -= 1;
    cardirection = false;
  }
}

void moveEnemies()
{

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

void handleCollisions()
{
  Rect player { carx, cary, carsize, carsize };
  
  Rect enemy { enemyx, enemyy, enemysize, enemysize };

  if (arduboy.collide(player, enemy))
  {
    gamestate = GAME_OVER;

    // No need to test any more collisions,
    // so exit the function early
    return;
  }

  Rect enemY { enemyX, enemyY, enemySize, enemySize };

  if (arduboy.collide (player, enemY))
  {
    gamestate = GAME_OVER;

    // No need to test any more collisions,
    // so exit the function early
    return;
  }

  Rect enemY2 (enemyX2, enemyY2, enemySize2, enemySize2);
}

void gameplay()
{
  // Defer to other functions,
  // to avoid thinking about too much at once.
  handlePlayerInput();
  moveEnemies();
  handleCollisions();
  drawGame();
}

void gameoverscreen() {
  
}

void highscorescreen() {
  if (arduboy.justPressed(B_BUTTON)) {
    gamestate = GAME_TITLE;
  }
}

void gameloop() {

  switch(gamestate) {
    
    case GAME_TITLE:
      titlescreen();
      break;

    case GAME_DELAY:
      updateGameDelayState();
      break;

    case GAME_PLAY:
      gameplay();
      break;

    case GAME_OVER:
      gameoverscreen();
      break;

    case GAME_HIGH:
      highscorescreen();
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
  if (!arduboy.nextFrame())
    return;
  
  arduboy.clear();
  arduboy.pollButtons();
  gameloop();
  arduboy.display();
}