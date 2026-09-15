#include <ESP32Servo.h>

//Servo Controls
Servo myServo;
int position = 0;
int position_1 = 90;
int direction = 0;
int turn = 0;

//PID Variables
float P = 0;
float I = 0;
float D = 0;

//PID Gain Variables
float kp = 1;
float ki = 0.5;
float kd = 0.01;

//PID Math Variables
float error = 0;
float error_1 = 0;
float error_2 = 0;

//PID Time Variables
float current_time = 0;
float previous_time = 0;
float time_change = 0;

//PID Control Variables
float right_sensor = 0;
float left_sensor = 0;
float input = 0;
float output = 0;
float output_1 = 0;
float goal = 8.190;

void setup() {
  //Defining Motor Pin
  myServo.attach(D5);
}

void loop() {
  //Defining Time Values
  current_time = micros();
  time_change = (current_time - previous_time) / 1e6;
  previous_time = current_time;
  
  //Reading Values From Photoresistors
  right_sensor = analogRead(D2);
  left_sensor = analogRead(D3);

  //Math for Input And Error
  input = abs((right_sensor + left_sensor) / 1000);
  error = goal - input;

  //Math For PID Controller Components
  P = kp * (error - error_1);
  I = ki * time_change * error_1;
  D = (kd / time_change) * (error_2 * error_1 + error_2);

  //Math For PID Output
  output = output_1 + P + I + D;
  if(abs(error) < 0.5){
    output = 0;
  }

  //Redefining Errors
  error_2 = error_1;
  error_1 = error;
  output_1 = output;

  //Printing Variables For Debugging
  Serial.print("Right: ");
  Serial.print(right_sensor);
  Serial.print(" left: ");
  Serial.print(left_sensor);
  Serial.print(" Input: ");
  Serial.print(input);
  Serial.print(" Error: ");
  Serial.print(error);
  Serial.print(" Output: ");
  Serial.print(output);
  Serial.print(" Position: ");
  Serial.print(position);
  Serial.println();

  //Motor Controls From PID Output
  //Motor Direction
  if(output != 0) {
    if((right_sensor - left_sensor) > 0){
      direction = 1;
    }
    else{
      direction = -1;
    }
    //Motor Movement
    position = position_1 + int(direction * output);
    if(position > 180){
      position = 180;
    }
    if(position < 0){
      position = 0;
    }
    position_1 = position;
  } 
  else {
    position = position_1;
  }

  //Motor Commands
  myServo.write(position);
  delay(100);
}
