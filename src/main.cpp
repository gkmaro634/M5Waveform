#include <Arduino.h>
#include <M5Unified.h>

#define WIDTH 80
#define HEIGHT 160
#define DELAY 500

M5GFX display;
M5Canvas canvas(&display);

int count = 0;

void setup() {
  M5.begin();
  display.init();

  canvas.createSprite(10, 20);

  display.fillScreen(BLACK);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (count % 2 == 0){
    canvas.fillSprite(BLACK);
  }
  else{
    canvas.fillSprite(BLUE);
  }

  display.startWrite();
  canvas.pushSprite(8, 8);
  display.endWrite();

  count++;
  delay(DELAY);
}
