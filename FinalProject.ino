#include <Keypad.h>
#include <Servo.h>

Servo sg90;

constexpr uint8_t greenLed = 8;
constexpr uint8_t redLed = 7;
constexpr uint8_t servoPin = 9;
constexpr uint8_t buzzerPin = 6;

char initial_password[4] = {'#', '0', '*', '9'};
char password[4];
char key_pressed = 0;
uint8_t i = 0;

const byte rows = 4;
const byte columns = 4;

char hexaKeys[rows][columns] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte row_pins[rows] = {A0, A1, A2, A3};
byte column_pins[columns] = {5, 4, 3, 2};

Keypad keypad_key = Keypad(makeKeymap(hexaKeys), row_pins, column_pins, rows, columns);

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);

  sg90.attach(servoPin);
  sg90.write(0);

  Serial.begin(9600);
}

void loop() {
  key_pressed = keypad_key.getKey();

  if (key_pressed) {
    password[i++] = key_pressed;
    Serial.print("*");
  }

  if (i == 4) {
    delay(200);
    boolean passwordCorrect = true;

    for (int j = 0; j < 4; j++) {
      if (password[j] != initial_password[j]) {
        passwordCorrect = false;
        break;
      }
    }

    if (passwordCorrect) {
      Serial.println("\nPass Accepted");
      sg90.write(90);
      digitalWrite(greenLed, HIGH);
      delay(3000);
      digitalWrite(redLed, LOW);
      //sg90.write(0);
      i=0;
    } else {
      Serial.println("\nWrong Password");
      digitalWrite(buzzerPin, HIGH);
      digitalWrite(redLed, HIGH);
      noTone(buzzerPin);
      delay(3000);
      //digitalWrite(buzzerPin, LOW);
      digitalWrite(redLed, LOW);
      i = 0;
    }
  }
}
