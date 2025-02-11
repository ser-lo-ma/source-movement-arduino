#include <UstepperS32.h>
UstepperS32 stepper;

#define STEPSPERREV 200 //Number of steps pr revolution. 200 for a 1.8deg motor, 400 for a 0.9deg motor
#define RES (STEPSPERREV * 256) / 360.0 //calculate microstep pr. degree
#define STEPPRMM 53.55 //full step pr. mm for the rail used in the demo
#define MMPRSTEP 1 / (STEPPRMM * 256) //mm pr. microstep
#define MMPRDEG MMPRSTEP * RES //mm pr. degree
#define STALLSENSITIVITY 8 //sensitivity of the stall detection, between -64 and 63 - higher number is less sensitive

// Desired rpm for homing
int16_t rpm = 50;
float railLength;

void setup() {
  stepper.setup(NORMAL, STEPSPERREV); //Initialize uStepper S32
  stepper.checkOrientation(30.0); //Check orientation of motor connector with +/- 30 microsteps movement
  stepper.setMaxAcceleration(2000); //Use an acceleration of 2000 fullsteps/s^2
  stepper.setMaxVelocity(500); //Max velocity of 500 fullsteps/s
  Serial.begin(9600);

  // Determine rail length in setup
  stepper.moveToEnd(CW, rpm, STALLSENSITIVITY); // Reset to ClockWise endpoint
  railLength = stepper.moveToEnd(CCW, rpm, STALLSENSITIVITY); // Go to Counter ClockWise end
  Serial.println("Rail length determined: " + String(railLength * MMPRDEG) + " mm");
}

void loop() {
  float partLength = railLength / 3.0; // Divide rail length into 3 parts

  for (int i = 0; i < 10; i++) {
    // Move to CW endpoint
    stepper.moveToEnd(CW, rpm, STALLSENSITIVITY);
    Serial.println("Moved to CW endpoint " + String(i+1) + "/10");

    // Move to middle part at half speed
    stepper.setMaxVelocity(250); // Half the speed
    stepper.moveSteps(partLength, CCW, STALLSENSITIVITY);
    Serial.println("Moved to middle part at half speed");

    // Move to CCW endpoint
    stepper.setMaxVelocity(500); // Restore full speed
    stepper.moveToEnd(CCW, rpm, STALLSENSITIVITY);
    Serial.println("Moved to CCW endpoint " + String(i+1) + "/10");

    // Move to middle part at half speed
    stepper.setMaxVelocity(250); // Half the speed
    stepper.moveSteps(partLength, CW, STALLSENSITIVITY);
    Serial.println("Moved to middle part at half speed");
  }
  while (1); // Stop the loop after 10 moves
}
