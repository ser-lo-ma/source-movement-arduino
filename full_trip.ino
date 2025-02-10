#include <UstepperS32.h>

#define LIMIT_SWITCH_PIN 4 // Define the pin connected to the limit switch

UstepperS32 stepper;
float limitPosition;

void setup() {
    // Initialize the limit switch pin
    pinMode(LIMIT_SWITCH_PIN, INPUT);

    // Initialize the stepper motor
    stepper.setup(NORMAL, 200); // Assuming 200 steps per revolution

    // Home the stepper motor
    stepper.moveToEnd(CW, 50, 2, 10000); // Move to the end in CW direction with stall detection
    stepper.encoder.setHome(); // Set the current position as home

    // Move until the limit switch is triggered
    moveToLimitSwitch(CCW, 50); // Move in CCW direction until the limit switch is triggered

    // Establish the limit position
    limitPosition = stepper.encoder.getAngleMoved();
}

void loop() {
    // Move to the limit position
    stepper.moveToAngle(limitPosition);
    delay(1000); // Wait for 1 second

    // Move back to the home position
    stepper.moveToAngle(0);
    delay(1000); // Wait for 1 second
}

void moveToLimitSwitch(bool dir, float rpm) {
    // Set the direction and RPM
    if (dir == CW) {
        stepper.setRPM(abs(rpm));
    } else {
        stepper.setRPM(-abs(rpm));
    }

    // Start moving the motor
    stepper.startMove();

    // Move until the limit switch is triggered
    while (digitalRead(LIMIT_SWITCH_PIN) == LOW) {
        delay(1);
    }

    // Stop the motor
    stepper.stop();
}