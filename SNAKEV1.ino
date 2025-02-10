/*
 * SNAKEV1
 * @author Joshua Weitzel, February 2025
 * This is an incomplete version of the final SNAKE game. It is simply a proof of concept and displays a controllable object on 
 * a display with a scorebaord and objects that can be picked up
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


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

class snake{
  //Player controls the snake
public:
  int x; 
  int y;
  int dx;
  int dy;
  int points;

  snake(){
    x = 64;
    y = 32;
    dx = 0;
    dy = 0;
    points = 0;
  }

  void draw(){
    //Position will change based on dy, dx
    x += dx;
    y += dy;
    //Draw circle
    display.fillCircle(x, y, 3, WHITE);
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

};

class pickup{
public:
  int x;
  int y;
  bool drawn;

  pickup(){
    x = 30;
    y = 30;
    drawn = false;
  }

  void draw(){
    if(!drawn){
      x = random(0, SCREEN_WIDTH - 5);
      y = random(20, SCREEN_HEIGHT - 5);
      drawn = true;
    }
    display.fillRect(x, y, 5, 5, WHITE);   
  }
};

//Construct objects
snake plyr;
pickup pick1;

void move(){
  int stateright = digitalRead(2);
  int statedown = digitalRead(3);
  int stateup = digitalRead(4);
  int stateleft = digitalRead(5);

  if(stateright == LOW){
    plyr.dx = 2;
    plyr.dy = 0;
  }
  if(statedown == LOW){
    plyr.dy = 2;
    plyr.dx = 0;
  }
  if(stateup == LOW){
    plyr.dy = -2;
    plyr.dx = 0;
  }
  if(stateleft == LOW){
    plyr.dx = -2;
    plyr.dy = 0;
  }
}

void scoreboard(){
  //Displays and updates scoreboard
  display.drawLine(0, 14, SCREEN_WIDTH, 14, WHITE);
  display.setCursor(SCREEN_WIDTH, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print(plyr.points);
}

void checkcollision(){
  if(plyr.x > pick1.x - 5 &&        // Player's left is left of pickup's right
      plyr.x - 3 < pick1.x + 5 &&  // Player's right is right of pickup's left
      plyr.y > pick1.y - 5 &&      // Player's top is above pickup's bottom
      plyr.y - 3 < pick1.y + 5) {  // Player's bottom is below pickup's top

    plyr.points++;
    pick1.drawn = false;
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
  display.clearDisplay();
  move();
  scoreboard();
  plyr.draw();
  pick1.draw();
  checkcollision();
  display.display();
  delay(50);
}
