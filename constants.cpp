#include "Arduino.h"

// ultrasonic
const int echoPin = 12;
const int trigPin = 13;

// ir sensor (non-pwm)
const int IR1 = 4;  // Kiri
const int IR2 = 3;  // Kiri tengah
const int IR3 = 2;  // Tengah kiri
const int IR4 = A3; // Tengah tengah
const int IR5 = A2; // Tengah kanan
const int IR6 = A1; // Kanan tengah
const int IR7 = A0; // Kanan

// motor dc
const int ENA = 5; // speed control motor A
const int ENB = 6; // speed control motor B
const int IN1 = 8;
const int IN2 = 9;
const int IN3 = 10;
const int IN4 = 11;

// servo
const int LEFT_SERVO = A4;
const int RIGHT_SERVO = A5;

// types
const int T_JUNCTION = 1;
const int LEFT_JUNCTION = 2;
const int RIGHT_JUNCTION = 3;