/*
   Boost Converter Powered By ATTiny84a Feedback.

   Program With Bootloader @1MHz For Max Stability.

   v1.0. 2020, OpenGC, phnx.
*/

#define FB_PIN A0
#define PWM_PIN 6
#define ERROR_LED_PIN 2
#define OK_LED_PIN 1

#define FB_RATIO 0.00166389 //feedback voltage divider ratio as set in hardware
#define TARGET_V 400 //target voltage
#define MAX_V 475

uint16_t pwmval = 0;

void setup() {
  analogReference(INTERNAL1V1); //Internal 1.1V Reference

  pinMode(FB_PIN, INPUT);
  pinMode(PWM_PIN, OUTPUT);
  pinMode(ERROR_LED_PIN, OUTPUT);
  pinMode(OK_LED_PIN, OUTPUT);

  // Set PWM pin to frequency of 31.65kHz
  TCCR1A = _BV(COM1A1) | _BV(WGM10);
  TCCR1B = _BV(CS10) | _BV(WGM12);

  digitalWrite(ERROR_LED_PIN, LOW);
  digitalWrite(OK_LED_PIN, LOW);
  digitalWrite(PWM_PIN, LOW);

  digitalWrite(ERROR_LED_PIN, HIGH);
  delay(200);
  digitalWrite(ERROR_LED_PIN, LOW);

  delay(100);
}

void loop() {
  uint16_t fbread = analogRead(FB_PIN);
  float fbvoltage = (1.1 / 1024.0) * float(fbread);
  float realvoltage = fbvoltage / float(FB_RATIO);

  // If above the maximum rated voltage, turn off immediately.
  //if (realvoltage > MAX_V) {
  //  error();
  //}

  if (realvoltage < TARGET_V) {
    if (pwmval < 253) {
      pwmval++;
    } else {
      ;//error();
    }
  } else if (realvoltage > TARGET_V) {
    if (pwmval > 2) {
      pwmval--;
    } else {
      ;//error();
    }
  } else {
    ;//reachedled = HIGH;
  }
  analogWrite(PWM_PIN, pwmval);
  //digitalWrite(OK_LED_PIN, reachedled);
  //delay(5);
}

void error() {
  analogWrite(PWM_PIN, 0);
  digitalWrite(PWM_PIN, LOW);
  digitalWrite(ERROR_LED_PIN, HIGH);
  while (1) {
    delay(50);
  }
}
