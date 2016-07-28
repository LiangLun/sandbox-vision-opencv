#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#define MOTOR_PIN_1 3
#define MOTOR_PIN_2 5
#define MOTOR_PIN_3 6
#define MOTOR_PIN_4 7
#define ENABLE_PIN  4
#define SPI_ENABLE_PIN 9
#define SPI_SELECT_PIN 10

const int role_pin = A4;

typedef enum {transmitter = 1, receiver} e_role;

unsigned long motor_value[2];

String input_string = "";
boolean string_complete = false;

RF24 radio(SPI_ENABLE_PIN, SPI_SELECT_PIN);

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

e_role role = receiver;

void setup() {
  pinMode(role_pin, INPUT);
  digitalWrite(role_pin, HIGH);
  delay(20);
  radio.begin();
  radio.setRetries(15, 15);
  Serial.begin(9600);
  Serial.println(" Setup Finished");
  if (digitalRead(role_pin)) {
    Serial.println(digitalRead(role_pin));
    role = transmitter;
  }
  else {
    Serial.println(digitalRead(role_pin));
    role = receiver;
  }

  if (role == transmitter) {
    
    radio.openWritingPipe(pipes[0]);
    radio.openReadingPipe(1, pipes[1]);
  }
  else {
    pinMode(MOTOR_PIN_1, OUTPUT);
    pinMode(MOTOR_PIN_2, OUTPUT);
    pinMode(MOTOR_PIN_3, OUTPUT);
    pinMode(MOTOR_PIN_4, OUTPUT);
    pinMode(ENABLE_PIN, OUTPUT);
    digitalWrite(ENABLE_PIN, HIGH);

    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1, pipes[0]);
  }

  radio.startListening();
}

void loop() {
  // TRANSMITTER CODE BLOCK //
  if (role == transmitter) {
    Serial.println("Transmitter");
    if (string_complete)
    {
      if (input_string == "Right!")
      {
        motor_value[0] = 0;
        motor_value[1] = 120;
      }
      else if (input_string == "Left!")
      {
        motor_value[0] = 120;
        motor_value[1] = 0;
      }
      else if (input_string == "Go!")
      {
        motor_value[0] = 120;
        motor_value[1] = 110;
      }
      else
      {
        motor_value[0] = 0;
        motor_value[1] = 0;
      }
      input_string = "";
      string_complete = false;
    }

    radio.stopListening();
    radio.write(motor_value, 2 * sizeof(unsigned long));
    radio.startListening();
    delay(20);
  }
  // RECEIVER CODE BLOCK //
  if (role == receiver) {
    Serial.println("Receiver");
    if (radio.available()) {
      bool done = false;
      while (!done) {
        done = radio.read(motor_value, 2 * sizeof(unsigned long));
        delay(20);
      }

      Serial.println(motor_value[0]);
      Serial.println(motor_value[1]);
      analogWrite(MOTOR_PIN_1, motor_value[1]);
      digitalWrite(MOTOR_PIN_2, LOW);
      analogWrite(MOTOR_PIN_3, motor_value[0]);
      digitalWrite(MOTOR_PIN_4  , LOW);
      radio.stopListening();
      radio.startListening();
    }
  }
}
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    input_string += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '!' || inChar == '?') {
      string_complete = true;
      Serial.print("data_received");
    }
  }
}
