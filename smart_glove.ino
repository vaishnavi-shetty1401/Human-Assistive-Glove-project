#include <Wire.h>
#include <Adafruit_ADXL345_U.h>
#include <LiquidCrystal_I2C_Hangul.h>
LiquidCrystal_I2C_Hangul lcd(0x27, 16, 2);
#include "DFRobotDFPlayerMini.h"
DFRobotDFPlayerMini myDFPlayer;
#include <SoftwareSerial.h>
SoftwareSerial Serial2(8, 9);

// Create an instance with default I2C address 0x53
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
const int relay1 = 7;
const int relay2 = 6;

void setup() {
  Serial.begin(115200);
  Serial.println("WELCOME");
  delay(200);
  Serial2.begin(9600);
  delay(200);
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  myDFPlayer.begin(Serial2);
  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("    WELCOME   ");
  delay(2000);
  // Start I2C with ESP32 default pins SDA=21, SCL=22
  // Wire.begin(21, 22);

  // Give sensor time to power up
  delay(100);

  // Initialize ADXL345
  if (!accel.begin()) {
    Serial.println("No ADXL345 detected, check wiring!");
    while (1);
  }

  Serial.println("ADXL345 detected!");

  // Set measurement range
  accel.setRange(ADXL345_RANGE_16_G);

  Serial.print("Range set to: ");
  switch (accel.getRange()) {
    case ADXL345_RANGE_2_G:  Serial.println("±2G"); break;
    case ADXL345_RANGE_4_G:  Serial.println("±4G"); break;
    case ADXL345_RANGE_8_G:  Serial.println("±8G"); break;
    case ADXL345_RANGE_16_G: Serial.println("±16G"); break;
  }
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  Serial.println(F("DFPlayer Mini online."));
  delay(4000);
  myDFPlayer.volume(30);  //Set volume value. From 0 to 30
  delay(100);
  myDFPlayer.EQ(1);
  delay(100);
  myDFPlayer.play(1); //play specific mp3 in SD:/MP3/0004.mp3; File Name(0~65535)
  delay(3000);
}

void readadxl() {
  sensors_event_t event;
  accel.getEvent(&event);

  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print(" m/s² ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print(" m/s² ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.println(" m/s²");
  delay(500);
  float x = event.acceleration.x;
  if (x >= -10 && x < -8) {
    Serial.println("Decision 1: Fan OFF");
    fanoff();
  }
  else if (x >= -8 && x < -6) {
    Serial.println("Decision 2: Fan ON");
    fanon();
  }
  else if (x >= -6 && x < -4) {
    Serial.println("Decision 3: Light ON");
    lighton();
  }
  else if (x >= -4 && x < -2) {
    Serial.println("Decision 4: Light OFF");
    lightoff();
  }
  else if (x >= -2 && x < 0) {
    Serial.println("Emergency");
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Emergency");
    myDFPlayer.play(2);
    delay(3000);
  }
  else if (x >= 0 && x < 2) {
    Serial.println("Iam Hungry");
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Iam Hungry");
    myDFPlayer.play(3);
    delay(3000);

  }
  else if (x >= 2 && x < 4) {
    Serial.println("What");
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("What");
    myDFPlayer.play(4);
    delay(3000);

  }
  else if (x >= 4 && x < 6) {
    Serial.println("I need Help");
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("I need Help");
    myDFPlayer.play(5);
    delay(3000);

  }
  else if (x >= 6 && x < 8) {
    Serial.println("Hello");
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Hello");
    myDFPlayer.play(6);
    delay(3000);

  }
  else if (x >= 8 && x <= 10) {
    Serial.println("I dont understand");
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("I dont understand");
    myDFPlayer.play(7);
    delay(3000);

  }

  delay(300);
}



void lighton()
{
  digitalWrite(relay1, HIGH);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Light ON");
  delay(2000);
}

void lightoff()
{
  digitalWrite(relay1, LOW);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Light OFF");
  delay(2000);
}

void fanon()
{
  digitalWrite(relay2, HIGH);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("FAN ON");
  delay(2000);
}

void fanoff()
{
  digitalWrite(relay2, LOW);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("FAN OFF");
  delay(2000);
}

void loop()
{
  readadxl();

}
