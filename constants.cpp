#include "Arduino.h"

// ultrasonic
const int echoPin = 12;
const int trigPin = 13;

// ir sensor (non-pwm)
const int IR1 = A0; // Kiri
const int IR2 = A1; // Kiri tengah
const int IR3 = A2; // Tengah kiri
const int IR4 = A2; // Tengah tengah
const int IR5 = A3; // Tengah kanan
const int IR6 = A3; // Kanan tengah
const int IR7 = A3; // Kanan

// motor dc
const int ENA = 5; // speed control motor A
const int ENB = 6; // speed control motor B
const int IN1 = 8; 
const int IN2 = 9;
const int IN3 = 10;
const int IN4 = 11;
