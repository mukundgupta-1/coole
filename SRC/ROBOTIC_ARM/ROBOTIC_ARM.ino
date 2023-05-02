#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;

void setup() {
  // Attach the servos to pins 9, 10, and 11
  servo1.attach(9);
  servo2.attach(10);
  servo3.attach(11);

  // Set the initial position of the servos to 90 degrees
  servo1.write(165);
  servo2.write(0);
  servo3.write(130);

  // Initialize the serial communication
  Serial.begin(9600);
}

void loop() {
  // Check if there is any input from the serial port
  if (Serial.available()) {
    // Read the input character
    char input = Serial.read();

    // Move the arm to the first point
    if (input == '1') {
      servo1.write(60);
      servo2.write(0);
      servo3.write(90);
    }
    // Move the arm to the second point
    else if (input == '2') {
      servo1.write(75);
      servo2.write(0);
      servo3.write(105);
    }
    // Move the arm to the third point
    else if (input == '3') {
      servo1.write(90);
      servo2.write(0);
      servo3.write(90);
    }
  }
}
