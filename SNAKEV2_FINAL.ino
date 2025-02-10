/*
 * SNAKEV2_FINAL
 * Author: Joshua Weitzel, February 2025
 */
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define I2C_ADDRESS   0x3C
#define BPINRIGHT     2
#define BPINDOWN      3
#define BPINUP        4
#define BPINLEFT      5
#define MSPEED        2
#define SNAKESIZE     5
#define OBJSIZE       8


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

class snake{
  //Player controls the snake
public:
  //Coordinates
  int x; 
  int y;
  //Speed
  int dx;
  int dy;
  //Player score
  int points;

  snake(){
    x = 64;
    y = 32;
    dx = 0;
    dy = 0;
    points = 0;
  }

  void drawhead(){
    //Position will change based on dy, dx
    x += dx;
    y += dy;
    //Draw circle
    display.fillCircle(x, y, SNAKESIZE, WHITE);
    //Screen wrap around
    if(x < 0){
      x = SCREEN_WIDTH;
    }
    if(x > SCREEN_WIDTH){
      x = 0;
    }
    if(y < 17){
      y = SCREEN_HEIGHT;
    }
    if(y > SCREEN_HEIGHT){
      y = 18;
    }
  }

  //Draw the snakes body segments
  //Segments are drawn to the previous i positions
  void drawbody(int* x, int* y){
    for(int i = 0; i < points; i++){
      display.fillCircle(x[i], y[i], SNAKESIZE, WHITE);
    }
  }
};

class pickup{
  //Player picks up squares to score points and grow the body
public:
  //Coordinates
  int x;
  int y;
  //Is there a pickup drawn on the screen
  bool drawn;

  pickup(){
    x = 30;
    y = 30;
    drawn = false;
  }

  //If there is not a pickup drawn on the screen, draw one
  void draw(){
    if(!drawn){
      x = random(0, SCREEN_WIDTH - 5);
      y = random(20, SCREEN_HEIGHT - 5);
      drawn = true;
    }
    display.fillRect(x, y, OBJSIZE, OBJSIZE, WHITE);   
  }
};

//Construct objects and intitiaize variables
snake plyr;   //Player
pickup pick1; //Objects to pickup

//Was going to use deques but arduino doesn't support it so we gotta get clever with arrays
//These bad boys are used store previous positions of the player to draw the snake body
int xloc[70];
int yloc[70];
bool gameover = false;
bool gamestart = false;
bool gamewin = false;


void move(){
  //move the player around the screen by changing its dy, dx values
  int stateright = digitalRead(2);
  int statedown = digitalRead(3);
  int stateup = digitalRead(4);
  int stateleft = digitalRead(5);

  if(stateright == LOW && plyr.dx == 0){
    plyr.dx = MSPEED;
    plyr.dy = 0;
  }
  if(statedown == LOW && plyr.dy == 0){
    plyr.dy = MSPEED;
    plyr.dx = 0;
  }
  if(stateup == LOW && plyr.dy == 0){
    plyr.dy = -MSPEED;
    plyr.dx = 0;
  }
  if(stateleft == LOW && plyr.dx == 0){
    plyr.dx = -MSPEED;
    plyr.dy = 0;
  }
}

bool anybutton(){
  //Check if any button is being pressed
  if(digitalRead(2) == LOW ||digitalRead(3) == LOW ||digitalRead(4) == LOW ||digitalRead(5) == LOW){
    return true;
  }
  return false;  
}

void scoreboard(){
  //Displays and updates scoreboard
  display.drawLine(0, 14, SCREEN_WIDTH, 14, WHITE);
  display.setCursor(SCREEN_WIDTH, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print(plyr.points);
}

void checkcollision(int* px, int* py, bool* collide){
  //check collision for point pickups
  if(plyr.x > pick1.x - OBJSIZE &&        // Player's left is left of pickup's right
      plyr.x - SNAKESIZE < pick1.x + OBJSIZE &&  // Player's right is right of pickup's left
      plyr.y > pick1.y - OBJSIZE &&      // Player's top is above pickup's bottom
      plyr.y - SNAKESIZE < pick1.y + OBJSIZE) {  // Player's bottom is below pickup's top

    plyr.points++;
    pick1.drawn = false;
  }

  //Check self collision for a game over
  //Will only check the segments past the first 5 to avoid issues with the head touching the body parts
  if(plyr.points > 5){
    for(int i = 4; i < plyr.points; i++){
      if(plyr.x > px[i] - (OBJSIZE - 3) &&        // Player's left is left of pickup's right
        plyr.x - (SNAKESIZE - 5) < px[i] + (OBJSIZE -3) &&  // Player's right is right of pickup's left
        plyr.y > py[i] - (OBJSIZE - 3) &&      // Player's top is above pickup's bottom
        plyr.y - (SNAKESIZE - 5) < py[i] + (OBJSIZE -3)) {  // Player's bottom is below pickup's top

        *collide = !(*collide);
        return;
      }
    }
  } 
}

void dynamicarray(int* x, int* y){
  //Shift elemnts in the arrays back to add new values
  for (int i = plyr.points; i > 0; i--) {
        x[i] = x[i - 1];
        y[i] = y[i - 1];
  }

  //Insert players current position at the front
  if(plyr.points > 0){
    x[0] = plyr.x;
    y[0] = plyr.y;
  }
}

void setup() {
  //Initialize screen
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, I2C_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  randomSeed(analogRead(0));
  //Enable button inputs
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
}

void loop() {
  //Display start screen
  while(!gamestart){
    display.setCursor(5, SCREEN_HEIGHT / 2 - 10);
    display.setTextSize(4);
    display.setTextColor(WHITE);
    display.print("START");
    display.display();
    if(anybutton() == true){
      gamestart = true;
    }
  }
  //Game has started, call functions every 50 milliseconds
  while(gamestart && !gameover && !gamewin){
    display.clearDisplay();
    move();
    scoreboard();
    dynamicarray(xloc, yloc);
    plyr.drawbody(xloc, yloc);
    plyr.drawhead();
    pick1.draw();
    checkcollision(xloc, yloc, &gameover);
    if(plyr.points == 69){
      gamewin = true;
    }

    display.display();
    delay(50);
  }
  //Snake hit its own body resulting in a game over, hold down any button for at least 1 second to restart
  while(gameover){
    display.clearDisplay();
    display.setCursor(10, SCREEN_HEIGHT /2 - 10);
    display.setTextSize(2);
    display.print("GAME OVER");
    display.display();
    plyr.points = 0;
    plyr.x = 30;
    plyr.y = 30;
    delay(1000);
    if(anybutton() == true){
      gameover = false;
    }
  }
  //Player scored the maximum points allowed by the arduino's RAM capacity, hold down any button for at least 1 second to play again
  while(gamewin){
    display.clearDisplay();
    display.setCursor(20, SCREEN_HEIGHT /2 - 10);
    display.setTextSize(2);
    display.print("YOU WIN");
    display.display();
    plyr.points = 0;
    plyr.x = 30;
    plyr.y = 30;
    delay(1000);
    if(anybutton() == true){
      gamewin = false;
    }
  }
}
