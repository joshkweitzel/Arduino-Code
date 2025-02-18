/*
 * PONG V1
 * Author: Joshua Weitzel, February 2025
 */
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define I2C_ADDRESS   0x3C
#define B_DOWN2       2
#define B_UP2         3
#define B_DOWN1       4
#define B_UP1         5
#define VB            3
#define VP            2
#define PADDLE_L      15
#define PADDLE_W      4
#define BALL_R        3

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

class Paddle {
  //Players each control a paddle
public:
  int x;
  int y;
  int points;

  Paddle(int xplace, int yplace){
    x = xplace;
    y = yplace;
    points = 0;
  }

  draw(){
    display.fillRect(x, y, PADDLE_W, PADDLE_L, WHITE);
  }

  move(int up, int down){
    if(up == LOW && y > 16){
      y -= VP;
    }
    if(down == LOW && y < SCREEN_HEIGHT - PADDLE_L){
      y += VP;
    }
  }
};

class Ball {
public:
  int x;
  int y;
  int dx;
  int dy;

  Ball(){
    //Spawns ball in the middle of the screen
    x = SCREEN_WIDTH / 2;
    y = SCREEN_HEIGHT / 2;
    //Starts the ball off not moving
    dx = 0;
    dy = 0;
  }

  draw(){
    //Draw ball
    display.fillCircle(x, y, BALL_R, WHITE);
    //Ball Ricochets off top and bottom
    if(y + BALL_R >= SCREEN_HEIGHT){
      dy *= -1;
      y -= 1;
    }
    if(y - BALL_R < 16){
      dy *= -1;
    }
    x += dx;
    y += dy;
  }
};

//Construct objects
Paddle plyr1(3, SCREEN_HEIGHT / 2);
Paddle plyr2(SCREEN_WIDTH - (PADDLE_W + 3), SCREEN_HEIGHT / 2);
Ball ball;

void ballupdate(){
  //If Ball has touched either side the opposite player has scored
  if(ball.x >= SCREEN_WIDTH){ //Ball touched right side
    ball.x = SCREEN_WIDTH / 2;
    ball.y = SCREEN_HEIGHT / 2;
    ball.dx *= -1;
    plyr1.points += 1;
  }
  if(ball.x<= 0){ //Ball touched left side
    ball.x = SCREEN_WIDTH / 2;
    ball.y = SCREEN_HEIGHT / 2;
    ball.dx *= -1;
    plyr2.points += 1;
  }
  
  //Ball ricochets off the paddles, gotta check for that collision
  //Check if ball is touching the right face of player 1's paddle
  if(ball.x - BALL_R <= plyr1.x + PADDLE_W &&
     ball.y - BALL_R >= plyr1.y &&
     ball.y + BALL_R <= plyr1.y + PADDLE_L){
    ball.dx *= -1;
    ball.dy *= -1;
  }
  //Check if ball is touching the left face of player 2's paddle
  if(ball.x + BALL_R >= plyr2.x - PADDLE_W &&
     ball.y - BALL_R >= plyr2.y &&
     ball.y + BALL_R <= plyr2.y + PADDLE_L){
    ball.dx *= -1;
    ball.dy *= -1;
  }
}

void scoreboard(){
  //Displays and updates scoreboard
  display.drawLine(0, 15, SCREEN_WIDTH, 15, WHITE);
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print(plyr1.points);
  if(plyr2.points < 10){
    display.setCursor(SCREEN_WIDTH - 12, 0);
  } else if(plyr2.points >= 10 && plyr2.points < 100){
    display.setCursor(SCREEN_WIDTH - 24, 0);
  } else if(plyr2.points >= 100){
    display.setCursor(SCREEN_WIDTH - 36, 0);
  }
  
  display.print(plyr2.points);
}

void setup() {
  //Initialize screen
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, I2C_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  //Randomizes the starting direction of the ball
  delay(50);
  randomSeed(analogRead(A0) + micros());
  ball.dx = (random(0, 10) >= 5) ? VB : -VB;
  ball.dy = (random(0, 10) >= 5) ? VB : -VB;
  //Enable button inputs
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
}

void loop() {
  display.clearDisplay();

  //Player 1 paddle methods
  plyr1.draw();
  plyr1.move(digitalRead(B_UP1), digitalRead(B_DOWN1));

  //Player 2 paddle methods
  plyr2.draw();
  plyr2.move(digitalRead(B_UP2), digitalRead(B_DOWN2));

  //Ball functions and methods
  ball.draw();
  ballupdate();


  //Graphical functions
  scoreboard();
  display.display();
  delay(50);
}
