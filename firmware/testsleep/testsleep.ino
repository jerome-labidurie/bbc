/* Deep Sleep - Blink
 *
 * Blinks the onboard LED, sleeps for 10 seconds and repeats
 *
 * Connections:
 * D0 -- RST
 *
 * If you cant reprogram as the ESP is sleeping, disconnect D0 - RST and try again
 */

// sleep for this many seconds
const int sleepSeconds = 5;

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
  Serial.begin(115200);
  Serial.println("\n\nWake up");

  // set led
  pinMode(BUILTIN_LED, OUTPUT);

  // Connect D0 to RST to wake up
  pinMode(D0, WAKEUP_PULLUP);

  Serial.println("Start blinking");
  blink (5, 250);
  Serial.println("Stop blinking");

  Serial.printf("Sleep for %d seconds\n\n", sleepSeconds);

  // convert to microseconds
  ESP.deepSleep(sleepSeconds * 1000000);
}

void loop() {
}
