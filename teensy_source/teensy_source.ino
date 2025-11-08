#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthNoisePink      pink1;          //xy=176.3333282470703,417.3333282470703
AudioFilterStateVariable filter1;        //xy=349,417.33331298828125
AudioMixer4              mixer1;         //xy=529,419.33331298828125
AudioEffectDelay         delay1;         //xy=705,473.33331298828125
AudioMixer4              mixer2;         //xy=858.3333129882812,353.33331298828125
AudioEffectFreeverb      reverb1;      //xy=999.3331909179688,381.3333435058594
AudioMixer4              mixer3;         //xy=1169.6666259765625,366.33331298828125
AudioOutputI2S           i2s_out;           //xy=1303.33349609375,368.3333435058594
AudioConnection          patchCord1(pink1, 0, filter1, 0);
AudioConnection          patchCord2(filter1, 0, mixer1, 0);
AudioConnection          patchCord3(filter1, 1, mixer1, 1);
AudioConnection          patchCord4(filter1, 2, mixer1, 2);
AudioConnection          patchCord5(mixer1, delay1);
AudioConnection          patchCord6(mixer1, 0, mixer2, 0);
AudioConnection          patchCord7(delay1, 0, mixer2, 1);
AudioConnection          patchCord8(mixer2, reverb1);
AudioConnection          patchCord9(mixer2, 0, mixer3, 0);
AudioConnection          patchCord10(reverb1, 0, mixer3, 1);
AudioConnection          patchCord11(mixer3, 0, i2s_out, 0);
AudioConnection          patchCord12(mixer3, 0, i2s_out, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=244.3333282470703,597.3333282470703
// GUItool: end automatically generated code

#define CUTOFF 20000
#define DELAY_MS 100
#define ROOMSIZE 1
#define DAMPING 1

void setup() {
  AudioMemory(100);
  sgtl5000_1.enable(); // Start SGTL5000
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN); // Select line in input
  sgtl5000_1.volume(0.1); // Set headphone volume level

  pink1.amplitude(1.0);
  
  filter1.frequency(CUTOFF);

  mixer1.gain(0, 1); // Low pass Output
  mixer1.gain(1, 0); // Band Pass Output
  mixer1.gain(2, 0); // High Pass Output

  delay1.delay(0, DELAY_MS);

  mixer2.gain(0, 1);
  mixer2.gain(0, 0);

  reverb1.roomsize(ROOMSIZE);
  reverb1.damping(DAMPING);

  mixer3.gain(0, 1);
  mixer3.gain(1, 0);
}

void loop() {


}
