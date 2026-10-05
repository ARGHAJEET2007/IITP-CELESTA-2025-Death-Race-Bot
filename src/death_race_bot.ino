
// --- RC channel input pins ---
#define CH2 A1   // Throttle
#define CH4 A2   // Steering

// --- Motor control pins (shared per side) ---
#define L_DIR 6
#define L_PWM 5   // must be PWM
#define R_DIR 10
#define R_PWM 9   // must be PWM

// --- Settings ---
const int deadzone   = 10;      // joystick neutral tolerance
const int maxPWM     = 250;     // max motor speed (0..255)
const unsigned long FAILSAFE_MS = 500;  // stop if no RC signal >0.5s

// --- Variables ---
int rawCh2 = 1500, rawCh4 = 1500;
int thr = 0, yaw = 0;
unsigned long lastSignalTime = 0;

// --- Read RC function ---
int readChannel(int pin, int minOut, int maxOut, int def, int &rawOut) {
  int us = pulseIn(pin, HIGH, 30000);   // measure PWM width (timeout 30ms)
  rawOut = us;
  if (us < 900 || us > 2100) return def; // invalid
  lastSignalTime = millis();             // update failsafe timer
  return map(us, 900, 2000, minOut, maxOut);
}

// --- Drive both sides ---
void setMotors(bool dirL, int spdL, bool dirR, int spdR) {
  digitalWrite(L_DIR, dirL ? HIGH : LOW);
  analogWrite(L_PWM, constrain(spdL, 0, 255));

  digitalWrite(R_DIR, dirR ? HIGH : LOW);
  analogWrite(R_PWM, constrain(spdR, 0, 255));
}

void stopMotors() {
  analogWrite(L_PWM, 0);
  analogWrite(R_PWM, 0);
}

void setup() {
  Serial.begin(9600);

  pinMode(CH2, INPUT);
  pinMode(CH4, INPUT);

  pinMode(L_DIR, OUTPUT);
  pinMode(L_PWM, OUTPUT);
  pinMode(R_DIR, OUTPUT);
  pinMode(R_PWM, OUTPUT);

  stopMotors();
  delay(1500);

  Serial.println("=== RC Car Debug (FS-i6S + Nano + 2xMDD10A + 2xMD10C) ===");
}

void loop() {
  // 1. Read RC channels
  thr = readChannel(CH2, -100, 100, 0, rawCh2); // throttle
  yaw = readChannel(CH4, -100, 100, 0, rawCh4); // steering

  // 2. Failsafe check
  if (millis() - lastSignalTime > FAILSAFE_MS) {
    stopMotors();
    Serial.println("!!! FAILSAFE TRIGGERED - No RC signal !!!");
    delay(50);
    return;
  }

  // 3. Deadzone
  if (abs(thr) < deadzone) thr = 0;
  if (abs(yaw) < deadzone) yaw = 0;

  // 4. Mixing (arcade drive)
  int leftCmd  = constrain(thr + yaw, -100, 100);
  int rightCmd = constrain(thr - yaw, -100, 100);

  bool leftDir  = (leftCmd >= 0);
  bool rightDir = (rightCmd >= 0);
  int  leftPWM  = map(abs(leftCmd),  0, 100, 0, maxPWM);
  int  rightPWM = map(abs(rightCmd), 0, 100, 0, maxPWM);

  // 5. Drive
  setMotors(leftDir, leftPWM, rightDir, rightPWM);

  // 6. Debug print
  Serial.print("CH2 raw=");
  Serial.print(rawCh2); Serial.print("us  Thr=");
  Serial.print(thr);

  Serial.print(" | CH4 raw=");
  Serial.print(rawCh4); Serial.print("us  Yaw=");
  Serial.print(yaw);

  Serial.print(" | Left=");
  Serial.print(leftDir ? "FWD" : "REV");
  Serial.print("@"); Serial.print(leftPWM);

  Serial.print("  Right=");
  Serial.print(rightDir ? "FWD" : "REV");
  Serial.print("@"); Serial.println(rightPWM);

  delay(30);
}
