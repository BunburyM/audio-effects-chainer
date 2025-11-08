#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s_in;           //xy=201.33331298828125,418.3333435058594
AudioFilterStateVariable filter;        //xy=349,417.33331298828125
AudioMixer4              mixer1;         //xy=529,419.33331298828125
AudioEffectDelay         delay;         //xy=705,473.33331298828125
AudioMixer4              mixer2;         //xy=858.3333129882812,353.33331298828125
AudioEffectFreeverb      reverb;      //xy=999.3331909179688,381.3333435058594
AudioMixer4              mixer3;         //xy=1169.6666259765625,366.33331298828125
AudioOutputI2S           i2s_out;           //xy=1303.33349609375,368.3333435058594
AudioConnection          patchCord1(i2s_in, 0, filter, 0);
AudioConnection          patchCord2(filter, 0, mixer1, 0);
AudioConnection          patchCord3(filter, 1, mixer1, 1);
AudioConnection          patchCord4(filter, 2, mixer1, 2);
AudioConnection          patchCord5(mixer1, delay);
AudioConnection          patchCord6(mixer1, 0, mixer2, 0);
AudioConnection          patchCord7(delay, 0, mixer2, 1);
AudioConnection          patchCord8(mixer2, reverb);
AudioConnection          patchCord9(mixer2, 0, mixer3, 0);
AudioConnection          patchCord10(reverb, 0, mixer3, 1);
AudioConnection          patchCord11(mixer3, 0, i2s_out, 0);
AudioConnection          patchCord12(mixer3, 0, i2s_out, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=244.3333282470703,597.3333282470703
// GUItool: end automatically generated code

#define CUTOFF 10000
#define DELAY_MS 100


void setup() {
  AudioMemory(100);
  sgtl5000_1.enable(); // Start SGTL5000
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN); // Select line in input
  sgtl5000_1.volume(0.5); // Set headphone volume level

  gain(0, 1); // Low pass Output
  gain(1, 0); // Band Pass Output
  gain(2, 0); // High Pass Output
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(ledPin, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
