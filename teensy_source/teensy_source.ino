#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s_in;           //xy=94.33332824707031,204.33334350585938
AudioFilterStateVariable filter1;        //xy=257,362.33331298828125
AudioMixer4              pan_mix;         //xy=270.33331298828125,232.3333740234375
AudioMixer4              mixer1;         //xy=384,371.33331298828125
AudioEffectWaveshaper    waveshape1;     //xy=455.33331298828125,487.3333435058594
AudioMixer4              mixer4;         //xy=617.3333129882812,364.3333435058594
AudioEffectDelay         delay1;         //xy=737,486.33331298828125
AudioMixer4              mixer2;         //xy=858.3333129882812,353.33331298828125
AudioEffectFreeverb      reverb1;      //xy=1017.333251953125,470.3333435058594
AudioMixer4              mixer3;         //xy=1169.6666259765625,366.33331298828125
AudioOutputI2S           i2s_out;           //xy=1303.33349609375,368.3333435058594
AudioConnection          patchCord1(i2s_in, 0, pan_mix, 0);
AudioConnection          patchCord2(i2s_in, 1, pan_mix, 1);
AudioConnection          patchCord3(filter1, 0, mixer1, 0);
AudioConnection          patchCord4(filter1, 1, mixer1, 1);
AudioConnection          patchCord5(filter1, 2, mixer1, 2);
AudioConnection          patchCord6(pan_mix, 0, filter1, 0);
AudioConnection          patchCord7(mixer1, waveshape1);
AudioConnection          patchCord8(mixer1, 0, mixer4, 0);
AudioConnection          patchCord9(waveshape1, 0, mixer4, 1);
AudioConnection          patchCord10(mixer4, delay1);
AudioConnection          patchCord11(mixer4, 0, mixer2, 0);
AudioConnection          patchCord12(delay1, 0, mixer2, 1);
AudioConnection          patchCord13(mixer2, reverb1);
AudioConnection          patchCord14(mixer2, 0, mixer3, 0);
AudioConnection          patchCord15(reverb1, 0, mixer3, 1);
AudioConnection          patchCord16(mixer3, 0, i2s_out, 0);
AudioConnection          patchCord17(mixer3, 0, i2s_out, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=244.3333282470703,597.3333282470703
// GUItool: end automatically generated code

#define CUTOFF 15000 // Max is 15kHz
#define DELAY_MS 100
#define ROOMSIZE 1
#define DAMPING 1

// Define Analog Pins
// Free A0, A2, A3, A8, A10-1, A11-1, A12-2, A13-2, A14-2, A15-2, A16, A17
// Free Digital pins 0, 1, 2, 3, 4, 5
#define pan_mix_pot A0
#define cutoff_pot A2
#define filter_mix_pot A3
#define waveshape_mix_pot A8
#define delay_ms_pot A10-1
#define delay_mix_pot A11-1
#define reverb_roomsize_pot A12-2
#define reverb_damping_pot A13-2
#define reverb_mix_pot A14-2

// Debounce buttons
Bounce filter_button = Bounce(0, 15);

void setup() {
  pinMode(0, INPUT_PULLUP); // Filter button
  AudioMemory(160);
  sgtl5000_1.enable(); // Start SGTL5000
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN); // Select line in input
  sgtl5000_1.volume(0.3); // Set headphone volume level

  pan_mix.gain(0, 0.5);
  pan_mix.gain(1, 0.5);

  filter1.frequency(CUTOFF);

  mixer1.gain(0, 1); // Low pass Output
  mixer1.gain(1, 0); // Band Pass Output
  mixer1.gain(2, 0); // High Pass Output

  delay1.delay(0, DELAY_MS);

  mixer2.gain(0, 1);
  mixer2.gain(0, 0);

  reverb1.roomsize(ROOMSIZE);
  reverb1.damping(DAMPING);

  mixer3.gain(0, 0.7);
  mixer3.gain(1, 0.3);
}

void loop() {


}
