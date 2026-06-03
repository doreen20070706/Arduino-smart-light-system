// ── 腳位定義 ────────────────────────────────────────────
const int micPin  = 2;    // 麥克風 D2（完全照原程式）
const int btnPin  = 3;    // 按鈕 D3
const int ledPin1 = 12;   // LED 1
const int ledPin2 = 11;   // LED 2
const int ledPin3 = 10;   // LED 3
const int ldrPin  = A1;   // 光敏電阻 A1

// ── 開關狀態 ─────────────────────────────────────────────
bool ledState     = false;
int  lastMicState = HIGH;
int  lastBtnState = HIGH;

// ── 光敏計時 ─────────────────────────────────────────────
unsigned long previousMillis = 0;
const long    LDR_INTERVAL   = 200;

// ── 關閉所有燈 ───────────────────────────────────────────
void allLedsOff() {
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
}

// ── 印出目前燈狀態 ────────────────────────────────────────
void printLedStatus(String source) {
  Serial.print("[");
  Serial.print(source);
  Serial.print("] 燈狀態：");
  Serial.println(ledState ? "開啟 (ON)" : "關閉 (OFF)");
}

// ── 依光敏值決定亮幾顆 ───────────────────────────────────
void updateLdrLeds() {
  int ldrValue = analogRead(ldrPin);
  int ledCount = constrain(map(ldrValue, 980, 1015, 1, 3), 1, 3);

  digitalWrite(ledPin1, HIGH);
  digitalWrite(ledPin2, ledCount >= 2 ? HIGH : LOW);
  digitalWrite(ledPin3, ledCount >= 3 ? HIGH : LOW);

  Serial.print("[光敏] LDR 值："); Serial.print(ldrValue);
  Serial.print("  亮燈數："); Serial.println(ledCount);
}

// ════════════════════════════════════════════════════════
void setup() {
  Serial.begin(9600);
  pinMode(micPin,  INPUT_PULLUP);  // 完全照原麥克風程式
  pinMode(btnPin,  INPUT_PULLUP);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  allLedsOff();
  Serial.println("系統啟動，燈狀態：關閉 (OFF)");
}

void loop() {
  int currentMicState = digitalRead(micPin);
  int currentBtnState = digitalRead(btnPin);

  // ── 麥克風：完全照原程式連擊偵測邏輯 ────────────────
  if (lastMicState == HIGH && currentMicState == LOW) {
    delay(15);
    int secondCheck = digitalRead(micPin);
    if (secondCheck == LOW) {
      ledState = !ledState;
      if (ledState) {
        updateLdrLeds();
      } else {
        allLedsOff();
      }
      printLedStatus("麥克風");
      delay(100);
    }
  }
  lastMicState = currentMicState;

  // ── 按鈕：完全照原按鈕程式下降緣邏輯 ────────────────
  if (lastBtnState == HIGH && currentBtnState == LOW) {
    delay(20);
    if (digitalRead(btnPin) == LOW) {
      ledState = !ledState;
      if (ledState) {
        updateLdrLeds();
      } else {
        allLedsOff();
      }
      printLedStatus("按鈕");
      delay(200);
    }
  }
  lastBtnState = currentBtnState;

  // ── 光敏：ledState 開啟時每 200ms 更新亮度 ───────────
  if (ledState) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= LDR_INTERVAL) {
      previousMillis = currentMillis;
      updateLdrLeds();
    }
  }
}
