/*
    This sketch will do all the pulse counting and user interface.

    v1.0. 2020, OpenGC, phnx.
*/

#include <Tiny4kOLED.h>

#include <EEPROM.h>
#include <PinChangeInterrupt.h>

#define BUZZER_PIN 1
#define LED_PIN 2
#define BAT_FB A0
#define BUZZER_INT 3
#define EVENT_INT 7

#define BUZZER_ADDR 0 //memory address for buzzer enable setting

#define REFRESH_INT 3 //refresh interval in s
#define EVENT_DELAY 1000 //event alarm on time in us

volatile bool buzzer_allow = true;
volatile uint16_t events = 0; //number of events

void setup() {
  analogReference(DEFAULT); //Use VCC as VREF, default.

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BAT_FB, INPUT);
  pinMode(BUZZER_INT, INPUT); //INPUT_PULLUP
  pinMode(EVENT_INT, INPUT); //INPUT_PULLUP

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, HIGH);
  delay(200);
  digitalWrite(LED_PIN, LOW);

  buzzer_allow = EEPROM.read(BUZZER_ADDR); //get last saved buzzer setting.

  oled.begin();
  oled.clear();
  oled.on();

  attachPCINT(digitalPinToPCINT(BUZZER_INT), toggle_buzzer, FALLING);
  attachPCINT(digitalPinToPCINT(EVENT_INT), register_event, RISING);
}

void loop() {
  /* Disable interrupt for the time being and update the display */
  detachPCINT(digitalPinToPCINT(EVENT_INT));
  update_display();
  attachPCINT(digitalPinToPCINT(EVENT_INT), register_event, RISING);

  delay(REFRESH_INT * 1000);
}

// Simply toggle the buzzer on and off
void toggle_buzzer() {
  buzzer_allow = !buzzer_allow;
  EEPROM.write(BUZZER_ADDR, buzzer_allow);
}

// Increment the event counter by one and signal a pulse
void register_event() {
  events++;

  if (buzzer_allow) {
    digitalWrite(BUZZER_PIN, HIGH);
  }
  digitalWrite(LED_PIN, HIGH);
  delayMicroseconds(EVENT_DELAY);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
}

// Display counts per minute and the battery voltage for now
void update_display() {
  uint16_t batread = analogRead(BAT_FB);
  float batv = (5.0 / 1024.0) * batread;

  float cps = float(events) / float(REFRESH_INT);
  events = 0;
  float cpm = cps * 60;

  oled.clear();
  oled.setFont(FONT8X16);
  oled.print(batv);
  oled.println(" V");
  oled.print(cpm);
  oled.println(" cpm");
}
