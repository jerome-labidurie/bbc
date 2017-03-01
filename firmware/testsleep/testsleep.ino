/* Deep Sleep - Blink
 *
 * Blinks the onboard LED, sleeps for 10 seconds and repeats
 *
 * Connections:
 * D0 -- RST
 * D1 : button input
 *
 * If you cant reprogram as the ESP is sleeping, disconnect D0 - RST and try again
 */

extern "C" {
#include "user_interface.h"

extern struct rst_info resetInfo;
}

// sleep for this many seconds
const int sleepSeconds = 5;

#define BUTTON D1

/** blink the builtin led
 *
 * usage:
 *  pinMode(BUILTIN_LED, OUTPUT);
 *  blink (5, 100);
 *
 * @param nb number of blinks
 * @param wait delay (ms) between blinks
 */
void blink (uint8_t nb, uint32_t wait) {
   // LED: LOW = on, HIGH = off
   for (int i = 0; i < nb; i++)
   {
      digitalWrite(BUILTIN_LED, LOW);
      delay(wait);
      digitalWrite(BUILTIN_LED, HIGH);
      delay(wait);
   }
}

void setup() {
//   struct rst_info * resetInfo = ESP.getResetInfoPtr();

  Serial.begin(115200);
  Serial.println("\n\nWake up");
  Serial.println (ESP.getResetReason());
  Serial.print ("reset:");
  Serial.println (resetInfo.reason);

  // set led
  pinMode(BUILTIN_LED, OUTPUT);
  // set button
  pinMode (BUTTON, INPUT);

  blink (2, 100);
  delay (500);

  // Connect D0 to RST to wake up
  pinMode(D0, WAKEUP_PULLUP);

  if (digitalRead(BUTTON) == 0) {
	  Serial.println ("Blink");
	  blink (5, 250);
  }


}

void loop() {
}
