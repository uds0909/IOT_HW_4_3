#include <Arduino.h>
#include <SSD1306.h>

SSD1306       display(0x3c, 4, 5, GEOMETRY_128_32);

const int pulseA = 12;
const int pulseB = 13;
const int pushSW = 2;
volatile int lastEncoded = 0;
volatile long encoderValue = 0;
const int RELAY = 15;
int val = 0;


void IRAM_ATTR handleRotary() {
 // Never put any long instruction
 int MSB = digitalRead(pulseA); //MSB = most significant bit
 int LSB = digitalRead(pulseB); //LSB = least significant bit
 int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
 int sum = (lastEncoded << 2) | encoded; //adding it to the previous encoded value
 if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
 if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;
 lastEncoded = encoded; //store this value for next time
 if (encoderValue > 255) {
 encoderValue = 255;
 } else if (encoderValue < 0 ) {
 encoderValue = 0;
 }
}

void IRAM_ATTR buttonClicked() {
 Serial.println("pushed");
}



void setup() {
  // put your setup code here, to run once:
  pinMode(RELAY,OUTPUT);
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);
  ////////////////////////////////////////
  pinMode(pushSW, INPUT_PULLUP);
  pinMode(pulseA, INPUT_PULLUP);
  pinMode(pulseB, INPUT_PULLUP);
  attachInterrupt(pushSW, buttonClicked, FALLING);
  attachInterrupt(pulseA, handleRotary, CHANGE);
  attachInterrupt(pulseB, handleRotary, CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:

  display.init();
  val = analogRead(0);
  display.drawString(0,0, "E : " + String(encoderValue));
  display.drawString(50,0, "S : " + String(val));
  display.display();
  if(val < encoderValue){
    digitalWrite(RELAY,HIGH);
    delay(500);
    digitalWrite(RELAY,LOW);
    delay(500);
  }
  delay(500);
}