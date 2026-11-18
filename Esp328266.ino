#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

// ================= DISPLAY =================
#define SCREEN_W 320
#define SCREEN_H 240

// ================= EYE SETTINGS =================
int eyeW = 85;
int eyeH = 105;

int leftEyeX  = 100;
int rightEyeX = 220;
int eyeY = 120;

int lookX = 0;
int lookY = 0;

// ================= TIMERS =================
unsigned long lastBlink = 0;
unsigned long lastLook = 0;
unsigned long lastSleepy = 0;

unsigned long blinkInterval = 3500;

bool sleepy = false;

// ================= DRAW ONE EYE =================
void drawEye(int x, int y, int w, int h, int offsetX, int offsetY) {

  // Fully closed eye
  if (h <= 8) {
    tft.fillRoundRect(
      x - w / 2, y - h / 2,
      w, h, 4, TFT_CYAN
    );
    return;
  }

  // Outer cyan eye
  tft.fillRoundRect(
    x - w / 2, y - h / 2,
    w, h, 24, TFT_CYAN
  );

  // Inner black area
  tft.fillRoundRect(
    x - w / 2 + 5, y - h / 2 + 5,
    w - 10, h - 10, 20, TFT_BLACK
  );

  // Pupil position
  int pupilX = x + offsetX;
  int pupilY = y + offsetY;

  // Cyan pupil
  tft.fillCircle(pupilX, pupilY, 23, TFT_CYAN);

  // Eye shine
  tft.fillCircle(pupilX - 8, pupilY - 8, 5, TFT_WHITE);
}

// ================= DRAW FACE =================
void drawFace() {
  tft.fillScreen(TFT_BLACK);

  drawEye(leftEyeX, eyeY, eyeW, eyeH, lookX, lookY);
  drawEye(rightEyeX, eyeY, eyeW, eyeH, lookX, lookY);
}

// ================= BLINK =================
void blinkEyes() {

  // Closing
  for (int h = eyeH; h >= 5; h -= 20) {
    drawEye(leftEyeX, eyeY, eyeW, h, lookX, lookY);
    drawEye(rightEyeX, eyeY, eyeW, h, lookX, lookY);
    delay(18);
  }

  // Closed
  drawEye(leftEyeX, eyeY, eyeW, 4, lookX, lookY);
  drawEye(rightEyeX, eyeY, eyeW, 4, lookX, lookY);
  delay(100);

  // Opening
  for (int h = 5; h <= eyeH; h += 20) {
    drawEye(leftEyeX, eyeY, eyeW, h, lookX, lookY);
    drawEye(rightEyeX, eyeY, eyeW, h, lookX, lookY);
    delay(18);
  }

  drawFace();
}

// ================= SLEEPY BLINK =================
void sleepyBlink() {

  // Half closed
  int halfH = eyeH / 2;

  drawEye(leftEyeX, eyeY, eyeW, halfH, lookX, lookY);
  drawEye(rightEyeX, eyeY, eyeW, halfH, lookX, lookY);

  delay(600);

  drawFace();
}

// ================= SETUP =================
void setup() {

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  randomSeed(micros());

  drawFace();

  lastBlink = millis();
  lastLook = millis();
  lastSleepy = millis();

  blinkInterval = random(2500, 5500);
}

// ================= LOOP =================
void loop() {

  unsigned long now = millis();

  // Random eye movement
  if (now - lastLook > random(2000, 4000)) {

    lastLook = now;

    int direction = random(0, 5);

    if (direction == 0) {
      lookX = -15;
      lookY = 0;
    }
    else if (direction == 1) {
      lookX = 15;
      lookY = 0;
    }
    else if (direction == 2) {
      lookX = 0;
      lookY = -10;
    }
    else {
      lookX = 0;
      lookY = 0;
    }

    drawFace();
  }

  // Normal blink
  if (now - lastBlink > blinkInterval) {

    lastBlink = now;
    blinkInterval = random(2500, 6000);

    blinkEyes();
  }

  // Occasional sleepy expression
  if (now - lastSleepy > 15000) {

    lastSleepy = now;

    if (random(0, 5) == 0) {
      sleepyBlink();
    }
  }
}