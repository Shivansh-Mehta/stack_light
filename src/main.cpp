#include <Arduino.h>

#define RED_PIN PIN_A4    // pin 18
#define YELLOW_PIN PIN_A5 // pin 19
#define GREEN_PIN PIN_A8  // pin 22

bool bRedPinFlag = false;
unsigned long periodMicro = 500000;
unsigned long refTime = 0;
uint8_t color = 0;

enum stackLightColor
{
  red = 1,
  yellow,
  green,
  disable,
};

void getUserInput();
void setUserInput();

void setup()
{
  Serial.begin(115200);

  // setting pins as output and at logic level low
  pinMode(RED_PIN, OUTPUT);
  digitalWrite(RED_PIN, LOW);
  pinMode(YELLOW_PIN, OUTPUT);
  digitalWrite(YELLOW_PIN, LOW);
  pinMode(GREEN_PIN, OUTPUT);
  digitalWrite(GREEN_PIN, LOW);
}

void loop()
{
  getUserInput();
  setUserInput();
}

void getUserInput()
{

  uint8_t localColor = 0;

  while (!Serial)
    ;
  Serial.println("Enter:");
  Serial.println("1\t:for red light");
  Serial.println("2\t:for yellow light");
  Serial.println("3\t:for green light");
  Serial.println("4\t:for disabling the stacking light");
  Serial.flush();

  while (Serial.available() == 0)
    ;
  localColor = Serial.parseInt();
  if (stackLightColor::red == localColor)
  {
    color = localColor;
    Serial.println("Color set: red");
    Serial.flush();
  }
  else if (stackLightColor::yellow == localColor)
  {
    color = localColor;
    Serial.println("Color set: yellow");
    Serial.flush();
  }
  else if (stackLightColor::green == localColor)
  {
    color = localColor;
    Serial.println("Color set: green");
    Serial.flush();
  }
  else if (stackLightColor::disable == localColor)
  {
    color = localColor;
    Serial.println("Stacking light disabled");
    Serial.flush();
  }
  else if (0 != localColor)
  {
    Serial.println("Incorrect input. Try again.");
    Serial.flush();
    getUserInput();
  }
  return;
}

void setUserInput()
{
  unsigned long updTime = micros();
  while ((updTime - refTime) >= (periodMicro))
  {
    refTime = updTime;

    digitalWrite(RED_PIN, LOW);
    digitalWrite(YELLOW_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);

    switch (color)
    {
    case stackLightColor::red:
      digitalWrite(RED_PIN, HIGH);
      break;
    case stackLightColor::yellow:
      digitalWrite(YELLOW_PIN, HIGH);
      break;
    case stackLightColor::green:
      digitalWrite(GREEN_PIN, HIGH);
      break;
    case stackLightColor::disable:
    {
      digitalWrite(RED_PIN, LOW);
      digitalWrite(YELLOW_PIN, LOW);
      digitalWrite(GREEN_PIN, LOW);
    }
    break;
    }
  }

  return;
}