#include "SoftwareSerial.h"
#include "rgb.h"
#include "gyro.h"
#include "domain/blade_state.h"

#define ONBOARD 13

#define BLADE_SWITCH 7

BladeState blade_state = OFF;

int buttonState = 0; // variable for reading the pushbutton status

#define AUDIO_TX_PIN 2
#define AUDIO_RX_PIN 4

#define LOOP_DELAY 10

SoftwareSerial mySoftwareSerial(AUDIO_TX_PIN, AUDIO_RX_PIN);
/*
RGB PRESETS
Red - (255, 0, 0)
Indigo - (15, 0, 255)
*/
rgb blade_color = rgb{255, 255, 0};
double time = 0;

Accelerometer accelerometer;

void setup()
{
  Serial.begin(115200);
  // on off button
  pinMode(BLADE_SWITCH, INPUT);
  pinMode(ONBOARD, OUTPUT);

  // RGB pin setup
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  // set up the accelerometer
  accelerometer.begin();
  accelerometer.calibrate();
  accelerometer.read(false);
}
void loop()
{
  // read the state of the pushbutton value:
  buttonState = getBladeButtonState();
  if (buttonState == HIGH)
  {
    do
    {
      buttonState = getBladeButtonState();
    } while (buttonState == HIGH);

    if (blade_state == OFF)
    {
      blade_state = IDLE;
    }
    else
    {
      blade_state = OFF;
    }
  }

  // turn on the blade if needed
  digitalWrite(ONBOARD, blade_state);
  blade_light(blade_state, blade_color);
  if (blade_state != OFF)
  {
    if (blade_state == BladeState::CLASH)
    {
      delay(100);
    }

    // Read the motion
    blade_state = accelerometer.get_blade_state(true);
  }

  // Print iter info
  // printInfo(get_frequency(time), time);
  time = int(time + 1) % 180;
  delay(LOOP_DELAY);
}

int getBladeButtonState()
{
  return digitalRead(BLADE_SWITCH);
}

void printInfo(int buttonState, int blade_state)
{
  Serial.print("Button State: ");
  Serial.print(buttonState);
  Serial.print("\r");
  Serial.print("Blade State:  ");
  Serial.print(blade_state);
  Serial.print("\r---------------\r");
}
