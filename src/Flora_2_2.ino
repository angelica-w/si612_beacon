/* 
 * Project myProject
 * Author: Your Name
 * Date: 
 * For comprehensive documentation and examples, please visit:
 * https://docs.particle.io/firmware/best-practices/firmware-template/
 */

// Include Particle Device OS APIs
#include "Particle.h"
#include "../lib/neopixel/src/neopixel.h"

#define PIXEL_PIN D2
// #define PIXEL_PIN SPI1
#define PIXEL_COUNT 12
#define PIXEL_TYPE WS2812


// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);

// Run the application and system concurrently in separate threads
SYSTEM_THREAD(ENABLED);

// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);

Adafruit_NeoPixel ring(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

unsigned long previousMillis = 0;
const unsigned long interval = 80;
const unsigned long onDuration = 5000;
bool allOn = false;
unsigned long allOnTimestamp = 0;
int currentPixel = 0;
// setup() runs once, when the device is first turned on
void setup() {
  // Put initialization like pinMode and begin functions here
  ring.begin();
  for (int i = 0; i < ring.numPixels(); i++) {
    ring.setPixelColor(i, ring.Color(255, 255, 255));
    ring.setBrightness(80);
  }
  ring.show();
  Particle.function("colorName", setNPColor);
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.

  // Example: Publish event to cloud every 10 seconds. Uncomment the next 3 lines to try it!
  // Log.info("Sending Hello World to the cloud!");
  // Particle.publish("Hello world!");
  // delay( 10 * 1000 ); // milliseconds and blocking - see docs for more info!
  unsigned long currentMillis = millis();

  if (allOn && currentMillis - allOnTimestamp >= onDuration) {
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      if (currentPixel < ring.numPixels()) {
        ring.setPixelColor(currentPixel, ring.Color(255, 255, 255));
        ring.show();
        currentPixel++;
      } else {
        allOn = false;
        currentPixel = 0;
      }
    }
  }
  // if (pixelsLit && millis() - lastUpdateTime >= ON_TIME) {
  //   // Turn off current pixel
  //   ring.setPixelColor(currentPixel, ring.Color(0, 0, 0));
  //   ring.show();
  //   currentPixel++;
  //   lastUpdateTime = millis();

  //   if (currentPixel >= PIXEL_COUNT) {
  //     // if all pixels off, reset state
  //     pixelsLit = false;
  //     currentPixel = 0;
  //   }
  // }


}

int setNPColor(String colorName) {
  uint8_t red = 0;
  uint8_t green = 0;
  uint8_t blue = 0;

  if (colorName == "yellow") {
    red = 255;
    green = 255;
    blue = 0;
  } else if (colorName == "purple") {
    red = 160;
    green = 32;
    blue = 240;
  } else if (colorName == "green") {
    red = 0;
    green = 255;
    blue = 0;
  } else if (colorName == "red") {
    red = 255;
    green = 0;
    blue = 0;
  } else {
    red = 0;
    green = 0;
    blue = 0;
  }

  for (int i = 0; i < ring.numPixels(); i++) {
    ring.setPixelColor(i, red, green, blue);
    ring.setBrightness(80);
    ring.show();
    delay(interval);
  }
  // ring.show();
  allOn = true;
  allOnTimestamp = millis();
  // lastUpdateTime = millis();
  currentPixel = 0;

  return 0;
}