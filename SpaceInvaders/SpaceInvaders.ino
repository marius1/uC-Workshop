#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define BUTTON_PIN 2

static const unsigned char PROGMEM enemy_bitmap[] =
{ B00011000,
  B00111100,
  B01111110,
  B11011011,
  B11111111,
  B00100100,
  B01011010,
  B10100101
};

int x_value = 0;
int y_value = 0;

unsigned char x = 0;
unsigned char y = 0;
unsigned char button_state;

#define MAX_BULLETS 1
#define BULLET_MIN_POS 16
#define BULLET_MAX_POS 58

unsigned char bullet_y_pos[MAX_BULLETS];
unsigned char bullet_x_pos[MAX_BULLETS];
unsigned char bullet = 0;
unsigned char bullet_available = 1;

unsigned int score = 0;
unsigned char lives = 5;

#define MAX_ENEMIES 1
#define ENEMY_MIN_POS 16
#define ENEMY_MAX_POS 64

unsigned char enemy_y_pos[MAX_ENEMIES];
unsigned char enemy_x_pos[MAX_ENEMIES];
unsigned char enemy = 0;
unsigned char enemy_available = 1;

void setup() {
  unsigned char i;
  Serial.begin(9600);

  pinMode(BUTTON_PIN, INPUT);
  digitalWrite(BUTTON_PIN, HIGH);

  randomSeed(analogRead(A2));

  for(i=0; i < MAX_ENEMIES; i++)
  {
    enemy_y_pos[i] = ENEMY_MAX_POS;
  }

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setTextWrap(false);
  display.display();

  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
}

void next_bullet()
{
  bullet = (bullet < MAX_BULLETS) ? bullet + 1 : 0;
}

void next_enemy()
{
  enemy = (enemy < MAX_ENEMIES) ? enemy + 1 : 0;
}

SIGNAL(TIMER0_COMPA_vect) 
{
  unsigned char i,j;
  unsigned long currentMillis = millis();

  if (currentMillis % 100 == 0)
  {
    enemy_available = 0;
    for(i = 0; i < MAX_ENEMIES; i++) 
    {
      for(j = 0; j < MAX_BULLETS; j++)
      {
        if ((bullet_x_pos[j] >= enemy_x_pos[i] && bullet_x_pos[j] <= enemy_x_pos[i] + 8)
         && (bullet_y_pos[j] >= enemy_y_pos[i] && bullet_y_pos[j] <= enemy_y_pos[i] + 8))
        {
          enemy_y_pos[j] = ENEMY_MAX_POS;
          enemy_available = 1;
          score++;
        }
      }
      
      if (enemy_y_pos[i] < ENEMY_MAX_POS) {
        enemy_y_pos[i]++;
        if( enemy_y_pos[i] == ENEMY_MAX_POS) {
          lives--;
        }
      } else {        
        enemy_y_pos[i] = ENEMY_MAX_POS;
        enemy_available = 1;
      }
    }
  }

  if (currentMillis % 25 == 0)
  {
    bullet_available = 0;
    for(i = 0; i < MAX_BULLETS; i++) 
    {
      if (bullet_y_pos[i] > BULLET_MIN_POS) {
        bullet_y_pos[i]--;
      } else {
        bullet_y_pos[i] = 0;
        bullet_available = 1;
      }
    }
  }
}

void loop() {
  unsigned char i, j;

  if (lives == 0) {
    game_over();
  }
  
  y_value = analogRead(A0);
  x_value = analogRead(A1);

  x = map(x_value, 0, 1023, 128, 0); 

  button_state = digitalRead(BUTTON_PIN); 
  if (button_state == 0 && bullet_available)
  {
    bullet_y_pos[bullet] = BULLET_MAX_POS;
    bullet_x_pos[bullet] = x;
    next_bullet();
  }

  if ( enemy_available )
  {
    if (random(15) == 1) 
    {
      enemy_y_pos[enemy] = ENEMY_MIN_POS;
      enemy_x_pos[enemy] = random(128);
      next_enemy();
    }
  }

  display.clearDisplay();  
  draw_score();

  // draw ship
  display.drawPixel(x, 59, WHITE);
  display.drawFastHLine(x-1, 60, 3, WHITE);
  display.drawFastHLine(x-3, 61, 7, WHITE);
  display.drawFastHLine(x-3, 62, 7, WHITE);
  display.drawPixel(x-3, 63, WHITE);
  display.drawPixel(x+3, 63, WHITE);

  // draw bullets if there
  for(i = 0; i < MAX_BULLETS; i++) 
  {
    if (bullet_y_pos[i] > BULLET_MIN_POS)
    {
      display.drawPixel(bullet_x_pos[i], bullet_y_pos[i], WHITE);
    }
  }

  // draw enemies if there
  for(i = 0; i < MAX_ENEMIES; i++)
  {
    if (enemy_y_pos[i] < ENEMY_MAX_POS)
    {
      //display.drawPixel(enemy_x_pos[i], enemy_y_pos[i], WHITE);
      display.drawBitmap(enemy_x_pos[i], enemy_y_pos[i], enemy_bitmap, 8, 8, WHITE);
    }
  }
  display.display();
}

void draw_score()
{
  display.setCursor(0,0);
  display.print("SCORE: ");
  display.print(score);
  display.setCursor(72, 0);
  display.print(" LIVES: ");
  display.print(lives);
}

void game_over()
{
  // draw score
  display.clearDisplay();
  draw_score();
  display.setTextSize(3);
  display.setCursor(28, 16);
  display.print("GAME");
  display.setCursor(28, 40);
  display.print("OVER");
  display.display();
  
  for(;;)
  {
  }
}

