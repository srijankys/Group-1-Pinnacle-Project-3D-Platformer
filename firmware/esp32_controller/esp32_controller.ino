/*
 * Group 1 - Pinnacle Project: ESP32 breadboard controller
 * Board : ESP32 Dev Module (USB-C), serial over USB (COM port), 115200 baud
 * Tool  : Arduino IDE -> Tools > Board > "ESP32 Dev Module"
 *
 * ---------------------------- PIN MAP ----------------------------
 *  Button      GPIO   Wire between GPIO and GND (internal pull-up used,
 *                     no resistor needed)         Game action / key
 *  LEFT        32                                  move_left   -> A
 *  RIGHT       33                                  move_right  -> D
 *  JUMP        25                                  jump        -> Space
 *  SHOOT       26                                  shoot_pad   -> J (auto-aim)
 *  ABILITY 1   27                                  ability_1   -> K
 *  ABILITY 2   14                                  ability_2   -> L
 *  ABILITY 3   13                                  ability_3   -> X
 *  ABILITY 4   23                                  ability_4   -> S
 *  Status LED  2  (on-board LED)                   lights while any button is held
 *  NOTE: change the numbers in BUTTONS[] below if your wiring differs.
 * -----------------------------------------------------------------
 *
 * Serial message format (one line per event, text, newline terminated):
 *     "<NAME>:<1|0>\n"      e.g.  "JUMP:1"  (pressed)   "JUMP:0"  (released)
 * On start-up and every 2 s the board also sends "HELLO" (heartbeat) so the
 * PC bridge can detect a dropped connection.
 */

struct Button {
  const char* name;       // text sent over serial
  uint8_t     pin;        // GPIO number
  bool        stable;     // debounced state (true = pressed)
  bool        lastRaw;    // last raw reading
  uint32_t    changedAt;  // time the raw reading last changed (ms)
};

Button BUTTONS[] = {
  {"LEFT",  32, false, false, 0},
  {"RIGHT", 33, false, false, 0},
  {"JUMP",  25, false, false, 0},
  {"SHOOT", 26, false, false, 0},
  {"AB1",   27, false, false, 0},
  {"AB2",   14, false, false, 0},
  {"AB3",   13, false, false, 0},
  {"AB4",   23, false, false, 0},
};
const int N_BUTTONS = sizeof(BUTTONS) / sizeof(BUTTONS[0]);

const uint8_t  LED_PIN       = 2;
const uint32_t DEBOUNCE_MS   = 15;    // ignore contact bounce shorter than this
const uint32_t HEARTBEAT_MS  = 2000;  // "HELLO" interval

uint32_t lastHeartbeat = 0;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  for (int i = 0; i < N_BUTTONS; i++) {
    pinMode(BUTTONS[i].pin, INPUT_PULLUP);  // pressed = LOW
  }
  Serial.println("HELLO");
}

void loop() {
  uint32_t now = millis();
  bool anyHeld = false;

  for (int i = 0; i < N_BUTTONS; i++) {
    Button &b = BUTTONS[i];
    bool raw = (digitalRead(b.pin) == LOW);   // LOW = pressed (pull-up wiring)

    if (raw != b.lastRaw) {                   // reading changed -> restart timer
      b.lastRaw = raw;
      b.changedAt = now;
    }
    // Accept the new state only after it has been steady for DEBOUNCE_MS
    if ((now - b.changedAt) >= DEBOUNCE_MS && raw != b.stable) {
      b.stable = raw;
      Serial.print(b.name);
      Serial.print(':');
      Serial.println(b.stable ? 1 : 0);
    }
    if (b.stable) anyHeld = true;
  }

  digitalWrite(LED_PIN, anyHeld ? HIGH : LOW);

  if (now - lastHeartbeat >= HEARTBEAT_MS) {  // connection heartbeat
    lastHeartbeat = now;
    Serial.println("HELLO");
  }
}
