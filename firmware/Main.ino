#include "thingProperties.h"

// ===== Motor Driver Pins =====

// Front motors (Driver 1)
#define ENA1 23
#define IN1A 22
#define IN2A 21
#define ENB1 19
#define IN3A 18
#define IN4A 5

// Rear motors (Driver 2)
#define ENA2 17
#define IN1B 16
#define IN2B 4
#define ENB2 15
#define IN3B 2
#define IN4B 13

// ===== Setup =====
void setup() {
  Serial.begin(115200);
  delay(1500);

  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  int pins[] = {IN1A, IN2A, IN3A, IN4A, IN1B, IN2B, IN3B, IN4B};
  for (int i = 0; i < 8; i++) pinMode(pins[i], OUTPUT);

  pinMode(ENA1, OUTPUT);
  pinMode(ENB1, OUTPUT);
  pinMode(ENA2, OUTPUT);
  pinMode(ENB2, OUTPUT);

  Serial.println("✅ ESP32 Omni Car Connected and Ready for Cloud Control");
}

void loop() {
  ArduinoCloud.update();
  handleButtons();
}

// ===== Cloud Variable Handlers =====
void onSpeedValueChange() {
  Serial.print("Speed set to ");
  Serial.println(speedValue);
}

void onForwardBtnChange()     { if (forwardBtn) moveForward(); }
void onBackwardBtnChange()    { if (backwardBtn) moveBackward(); }
void onLeftBtnChange()        { if (leftBtn) moveLeft(); }
void onRightBtnChange()       { if (rightBtn) moveRight(); }
void onRotateLeftBtnChange()  { if (rotateLeftBtn) rotateLeft(); }
void onRotateRightBtnChange() { if (rotateRightBtn) rotateRight(); }
void onStopBtnChange()        { if (stopBtn) stopMotors(); }

// ===== Button Logic =====
void handleButtons() {
  if (forwardBtn) moveForward();
  else if (backwardBtn) moveBackward();
  else if (leftBtn) moveLeft();
  else if (rightBtn) moveRight();
  else if (rotateLeftBtn) rotateLeft();
  else if (rotateRightBtn) rotateRight();
  else if (stopBtn) stopMotors();
  else stopMotors();
}

// ===== Motion Functions =====
void moveForward() {
  int spd = map(speedValue, 0, 100, 100, 255);
  motorA(true, spd);
  motorB(true, spd);
  motorC(true, spd);
  motorD(true, spd);
  Serial.println("⬆️ Forward");
}

void moveBackward() {
  int spd = map(speedValue, 0, 100, 100, 255);
  motorA(false, spd);
  motorB(false, spd);
  motorC(false, spd);
  motorD(false, spd);
  Serial.println("⬇️ Backward");
}

void moveLeft() {
  int spd = map(speedValue, 0, 100, 100, 255);
  motorA(false, spd);
  motorB(true, spd);
  motorC(true, spd);
  motorD(false, spd);
  Serial.println("⬅️ Left");
}

void moveRight() {
  int spd = map(speedValue, 0, 100, 100, 255);
  motorA(true, spd);
  motorB(false, spd);
  motorC(false, spd);
  motorD(true, spd);
  Serial.println("➡️ Right");
}

void rotateLeft() {
  int spd = map(speedValue, 0, 100, 100, 255);
  motorA(false, spd);
  motorB(true, spd);
  motorC(false, spd);
  motorD(true, spd);
  Serial.println("↩️ Rotate Left");
}

void rotateRight() {
  int spd = map(speedValue, 0, 100, 100, 255);
  motorA(true, spd);
  motorB(false, spd);
  motorC(true, spd);
  motorD(false, spd);
  Serial.println("↪️ Rotate Right");
}

void stopMotors() {
  analogWrite(ENA1, 0);
  analogWrite(ENB1, 0);
  analogWrite(ENA2, 0);
  analogWrite(ENB2, 0);

  digitalWrite(IN1A, LOW);
  digitalWrite(IN2A, LOW);
  digitalWrite(IN3A, LOW);
  digitalWrite(IN4A, LOW);
  digitalWrite(IN1B, LOW);
  digitalWrite(IN2B, LOW);
  digitalWrite(IN3B, LOW);
  digitalWrite(IN4B, LOW);

  Serial.println("🛑 STOP");
}

// ===== Motor Helpers =====
void motorA(bool forward, int spd) { setMotor(IN1A, IN2A, ENA1, spd, forward); } // Front Left
void motorB(bool forward, int spd) { setMotor(IN3A, IN4A, ENB1, spd, forward); } // Front Right
void motorC(bool forward, int spd) { setMotor(IN1B, IN2B, ENA2, spd, forward); } // Rear Left
void motorD(bool forward, int spd) { setMotor(IN3B, IN4B, ENB2, spd, forward); } // Rear Right

void setMotor(int in1, int in2, int en, int spd, bool forward) {
  digitalWrite(in1, forward ? HIGH : LOW);
  digitalWrite(in2, forward ? LOW : HIGH);
  analogWrite(en, spd);
}

/*
  Since Direction is READ_WRITE variable, onDirectionChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onDirectionChange()  {
  // Add your code here to act upon Direction change
}
/*
  Since CarActive is READ_WRITE variable, onCarActiveChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onCarActiveChange()  {
  // Add your code here to act upon CarActive change
}
