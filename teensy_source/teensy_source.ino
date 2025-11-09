#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s_in;           //xy=94.33332824707031,204.33334350585938
AudioMixer4              pan_mix;         //xy=229.3332977294922,233.3333740234375
AudioFilterStateVariable filter1;        //xy=252,366.33331298828125
AudioMixer4              filter_mix;         //xy=384,371.33331298828125
AudioEffectBitcrusher    bitcrush1;    //xy=489,499.3333435058594
AudioMixer4              bitcrush_mix;         //xy=617.3333129882812,364.3333435058594
AudioEffectDelay         delay1;         //xy=737,486.33331298828125
AudioMixer4              delay_mix;         //xy=858.3333129882812,353.33331298828125
AudioEffectFreeverb      reverb1;      //xy=944.333251953125,469.3333435058594
AudioMixer4              reverb_mix;         //xy=1066.6666259765625,362.33331298828125
AudioMixer4              bypass_mix;         //xy=1214.3333740234375,291.3333435058594
AudioAmplifier           amp1;           //xy=1332.3809356689453,373.3333168029785
AudioOutputI2S           i2s_out;           //xy=1504.762092590332,366.90478134155273
AudioConnection          patchCord1(i2s_in, 0, pan_mix, 0);
AudioConnection          patchCord2(i2s_in, 1, pan_mix, 1);
AudioConnection          patchCord3(pan_mix, 0, filter1, 0);
AudioConnection          patchCord4(pan_mix, 0, bypass_mix, 0);
AudioConnection          patchCord5(filter1, 0, filter_mix, 0);
AudioConnection          patchCord6(filter1, 1, filter_mix, 1);
AudioConnection          patchCord7(filter1, 2, filter_mix, 2);
AudioConnection          patchCord8(filter_mix, 0, bitcrush_mix, 0);
AudioConnection          patchCord9(filter_mix, bitcrush1);
AudioConnection          patchCord10(bitcrush1, 0, bitcrush_mix, 1);
AudioConnection          patchCord11(bitcrush_mix, delay1);
AudioConnection          patchCord12(bitcrush_mix, 0, delay_mix, 0);
AudioConnection          patchCord13(delay1, 0, delay_mix, 1);
AudioConnection          patchCord14(delay_mix, reverb1);
AudioConnection          patchCord15(delay_mix, 0, reverb_mix, 0);
AudioConnection          patchCord16(reverb1, 0, reverb_mix, 1);
AudioConnection          patchCord17(reverb_mix, 0, bypass_mix, 1);
AudioConnection          patchCord18(bypass_mix, amp1);
AudioConnection          patchCord19(amp1, 0, i2s_out, 0);
AudioConnection          patchCord20(amp1, 0, i2s_out, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=254.3333282470703,515.3333435058594
// GUItool: end automatically generated code

// Define Analog Pins
// Free A0, A2, A3, A8, A10-1, A11-1, A12-2, A13-2, A14-2, A15-2, A16, A17
// Free Digital pins 0, 1, 2, 3, 4, 5
#define master_volume_pot A1
#define cutoff_pot A2
#define bitcrush_mix_pot A8
#define bitcrush_bits_pot A10-1
#define bitcrush_samplerate_pot A11-1
#define delay_ms_pot A12-2
#define delay_mix_pot A13-2
#define reverb_roomsize_pot A14-2
#define reverb_damping_pot A15-2 
#define reverb_mix_pot A16

// Debounce buttons
Bounce filter_button = Bounce(0, 10);
Bounce bypass_button = Bounce(1, 10);

// Variables
float cutoff_freq_val = 20000; // 0 to 20000 Hz
float bitcrush_mix_val = 0; // 0 to 1
uint8_t bitcrush_bits_val = 16; // 1 to 16 bits
float bitcrush_samplerate_val = 44100; // 1 to 44100 Hz
float delay_ms_val = 100; // 0 to 1000 ms
float delay_mix_val = 0; // 0 to 1
float reverb_roomsize_val = 0; // 0 to 1
float reverb_damping_val = 0; // 0 to 1
float reverb_mix_val = 0; // 0 to 1
float main_signal_val = 0; // 0 to 1
float master_volume_val = 0; // 0 to 1
int filter_button_state = 0; // 0 to 1
int filter_choose_val = 0; // 0 to 2
int bypass_switch_state = 0; // 0 to 1
int filter_changed = 1; // 0 to 1

void setup() 
{
  Serial.begin(9600);
  pinMode(0, INPUT_PULLUP); // Filter choice button
  pinMode(1, INPUT_PULLUP); // Bypass Mode

  AudioMemory(1500);
  sgtl5000_1.enable(); // Start SGTL5000
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN); // Select line in input
  
  // Does not change, both left and right channels are balanced
  pan_mix.gain(0, 0.5); // Left
  pan_mix.gain(1, 0.5); // Right

  filter1.frequency(cutoff_freq_val);

  filter_mix.gain(0, 1); // Low pass Output
  filter_mix.gain(1, 0); // Band Pass Output
  filter_mix.gain(2, 0); // High Pass Output

  bitcrush1.bits(bitcrush_bits_val);
  bitcrush1.sampleRate(bitcrush_samplerate_val);

  bitcrush_mix.gain(0, 1); // Original signal
  bitcrush_mix.gain(1, 0); // Bitcrush Mix

  delay1.delay(0, delay_ms_val);

  delay_mix.gain(0, 1); // Original signal
  delay_mix.gain(0, 0); // Delay mix

  reverb1.roomsize(reverb_roomsize_val);
  reverb1.damping(reverb_damping_val);

  reverb_mix.gain(0, 1); // Original signal
  reverb_mix.gain(1, 0); // Reverb Mix

  sgtl5000_1.volume(1); // Set headphone volume level
  amp1.gain(1);
}

void loop() 
{
  /*  FREQUENCY SECTION   */
  cutoff_freq_val = analog_map(cutoff_pot, 0, 20000);
  filter1.frequency(cutoff_freq_val);
  filter_button_state = digitalRead(0);
  // See if user wants to switch filter
  if(filter_button_state == LOW && filter_changed == 1)
  {
    switch_filter();
    filter_changed = 0;
  }

  else if(filter_button_state == HIGH)
  {
    filter_changed = 1;
  }
  
  /*  BITCRUSHER SECTION   */
  bitcrush_bits_val = (uint8_t)analog_map(bitcrush_bits_pot, 1, 16);
  bitcrush1.bits(bitcrush_bits_val);

  bitcrush_samplerate_val = analog_map(bitcrush_samplerate_pot, 1, 44100);
  bitcrush1.sampleRate(bitcrush_samplerate_val);

  bitcrush_mix_val = analog_map(bitcrush_mix_pot, 0, 1);
  main_signal_val = 1 - bitcrush_mix_val;
  bitcrush_mix.gain(0, main_signal_val); // Original
  bitcrush_mix.gain(1, bitcrush_mix_val); // Bitcrush

  /*  DELAY SECTION   */
  delay_ms_val = analog_map(delay_ms_pot, 0, 1000);
  delay1.delay(0, delay_ms_val);

  delay_mix_val = analog_map(delay_mix_pot, 0, 1);
  main_signal_val = 1 - delay_mix_val;
  delay_mix.gain(0, main_signal_val); // Original
  delay_mix.gain(1, delay_mix_val); // Delay

  /*  DELAY SECTION   */
  reverb_roomsize_val = analog_map(reverb_roomsize_pot, 0, 1);
  reverb1.roomsize(reverb_roomsize_val);

  reverb_damping_val = analog_map(reverb_damping_pot, 0, 1);
  reverb1.damping(reverb_damping_val);

  reverb_mix_val = analog_map(reverb_mix_pot, 0, 1);
  main_signal_val = 1 - reverb_mix_val;
  reverb_mix.gain(0, main_signal_val); // Original
  reverb_mix.gain(1, reverb_mix_val); // Reverb

  /*  VOLUME AND BYPASS SECTION  */
  master_volume_val = analog_map(master_volume_pot, 0, 1);
  amp1.gain(master_volume_val);

  // Check if bypass mode is on
  bypass_switch_state = digitalRead(1);
  while(bypass_switch_state == LOW)
  {
    bypass_mix.gain(0, 1); // Clean signal
    bypass_mix.gain(1, 0); // Effects signal
    master_volume_val = analog_map(master_volume_pot, 0, 1);
    amp1.gain(master_volume_val);
    bypass_switch_state = digitalRead(1);
  }

  bypass_mix.gain(0, 0); // Clean signal
  bypass_mix.gain(1, 1); // Effects signal
}

// Map analog value from pot to digital value
// Returns mapped digital float value
float analog_map(int pot, float range_low, float range_high)
{
  float value = analogRead(pot);
  float result =  (value - 0.0)/(1023.0 - 0.0)*(range_high-range_low)+range_low;
  return result;
}

// Switch between a low-pass, high-pass, and band-pass filter
void switch_filter()
{
  // Update value to choose from low-pass, high-pass, or band-pass
  filter_choose_val = (filter_choose_val + 1) % 3; 
  
  switch(filter_choose_val)
  {
    // Low Pass Filter
    case 0:
      filter_mix.gain(0, 1); // Low pass Output
      filter_mix.gain(1, 0); // Band Pass Output
      filter_mix.gain(2, 0); // High Pass Output
      break;

    // High Pass Filter
    case 1:
      filter_mix.gain(0, 0); // Low pass Output
      filter_mix.gain(1, 0); // Band Pass Output
      filter_mix.gain(2, 1); // High Pass Output
      break;

    // Band Pass Filter
    case 2:
      filter_mix.gain(0, 0); // Low pass Output
      filter_mix.gain(1, 1); // Band Pass Output
      filter_mix.gain(2, 0); // High Pass Output
      break;
  }
}
