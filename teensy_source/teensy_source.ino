#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Bounce.h>


// GUItool: begin automatically generated code
AudioInputI2S            i2s_in;         //xy=109,468
AudioMixer4              pan_mix;        //xy=257,357
AudioFilterLadder        filter1;        //xy=292,603
AudioEffectBitcrusher    bitcrush1;      //xy=435,709
AudioMixer4              bitcrush_mix;   //xy=484,480
AudioEffectDelay         delay1;         //xy=707,685
AudioMixer4              delay_mix;      //xy=718,471
AudioEffectFreeverb      reverb1;        //xy=959,733
AudioMixer4              reverb_mix;     //xy=1081,626
AudioMixer4              bypass_mix;     //xy=1239,357
AudioAmplifier           amp1;           //xy=1347,637
AudioOutputI2S           i2s_out;        //xy=1519,630
AudioConnection          patchCord1(i2s_in, 0, pan_mix, 0);
AudioConnection          patchCord2(i2s_in, 1, pan_mix, 1);
AudioConnection          patchCord3(pan_mix, 0, bypass_mix, 0);
AudioConnection          patchCord4(pan_mix, 0, filter1, 0);
AudioConnection          patchCord5(filter1, 0, bitcrush_mix, 0);
AudioConnection          patchCord6(filter1, bitcrush1);
AudioConnection          patchCord7(bitcrush1, 0, bitcrush_mix, 1);
AudioConnection          patchCord8(bitcrush_mix, 0, delay_mix, 0);
AudioConnection          patchCord9(delay1, 0, reverb_mix, 0);
AudioConnection          patchCord10(delay1, 0, delay_mix, 1);
AudioConnection          patchCord11(delay1, 0, reverb1, 0);
AudioConnection          patchCord12(delay_mix, delay1);
AudioConnection          patchCord13(reverb1, 0, reverb_mix, 1);
AudioConnection          patchCord14(reverb_mix, 0, bypass_mix, 1);
AudioConnection          patchCord15(bypass_mix, amp1);
AudioConnection          patchCord16(amp1, 0, i2s_out, 0);
AudioConnection          patchCord17(amp1, 0, i2s_out, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=213,782
// GUItool: end automatically generated code








// Define Analog Pins
// Free A0, A2, A3, A8, A10-1, A11-1, A12-2, A13-2, A14-2, A15-2, A16, A17
// Free Digital pins 0, 1, 2, 3, 4, 5
#define master_volume_pot A1
#define cutoff_pot A2
#define cutoff_resonance_pot A3
#define bitcrush_mix_pot A8
#define bitcrush_bits_pot A10
#define bitcrush_samplerate_pot A11
#define delay_ms_pot A12
#define delay_mix_pot A13
#define reverb_roomsize_pot A14
#define reverb_damping_pot A15
#define reverb_mix_pot A16

// Variables
float cutoff_freq_val = 20000; // 0 to 20000 Hz
float cutoff_resonance_val = 0; // 0 to 1.8
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
int bypass_switch_state = 0; // 0 to 1

void setup() 
{
  Serial.begin(9600);
  pinMode(1, INPUT_PULLUP); // Bypass Mode

  AudioMemory(1500);
  sgtl5000_1.enable(); // Start SGTL5000
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN); // Select line in input
  
  // Does not change, both left and right channels are balanced
  pan_mix.gain(0, 0.5); // Left
  pan_mix.gain(1, 0.5); // Right

  filter1.frequency(cutoff_freq_val);
  filter1.resonance(cutoff_resonance_val);
  filter1.inputDrive(1);
  filter1.octaveControl(0);
  filter1.passbandGain(0.5);

  bitcrush1.bits(bitcrush_bits_val);
  bitcrush1.sampleRate(bitcrush_samplerate_val);

  bitcrush_mix.gain(0, 1); // Original signal
  bitcrush_mix.gain(1, 0); // Bitcrush Mix

  delay1.delay(0, delay_ms_val);
  delay1.disable(1);
  delay1.disable(2);
  delay1.disable(3);
  delay1.disable(4);
  delay1.disable(5);
  delay1.disable(6);
  delay1.disable(7);

  delay_mix.gain(0, 1); // Original signal
  delay_mix.gain(1, 0); // Delay mix

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
  cutoff_freq_val = int(analog_map(cutoff_pot, 0, 20000)/100+0.5)*100;
  filter1.frequency(cutoff_freq_val);
  Serial.println("cutoff frequency " + String(cutoff_freq_val));
  cutoff_resonance_val = analog_map(cutoff_resonance_pot, 0, 1.8);
  filter1.resonance(cutoff_resonance_val);
  
  /*  BITCRUSHER SECTION   */
  bitcrush_bits_val = (uint8_t)analog_map(bitcrush_bits_pot, 1, 16);
  bitcrush1.bits(bitcrush_bits_val);

  bitcrush_samplerate_val = analog_map(bitcrush_samplerate_pot, 1, 44100);
  bitcrush1.sampleRate(bitcrush_samplerate_val);

  bitcrush_mix_val = analog_map(bitcrush_mix_pot, 0, 1);
  main_signal_val = 1 - bitcrush_mix_val;
  bitcrush_mix.gain(0, main_signal_val); // Original
  bitcrush_mix.gain(1, bitcrush_mix_val); // Bitcrush
  Serial.println("bitcrush value " + String(bitcrush_mix_val));

  /*  DELAY SECTION   */
  delay_ms_val = analog_map(delay_ms_pot, 0, 1000);
  delay1.delay(0, delay_ms_val);

  delay_mix_val = analog_map(delay_mix_pot, 0, 1);
  main_signal_val = 1 - delay_mix_val;
  delay_mix.gain(0, main_signal_val); // Original
  delay_mix.gain(1, delay_mix_val); // Delay
  Serial.println("delay value " + String( delay_mix_val));

  /*  REVERB SECTION   */
  reverb_roomsize_val = analog_map(reverb_roomsize_pot, 0, 1);
  reverb1.roomsize(reverb_roomsize_val);

  reverb_damping_val = analog_map(reverb_damping_pot, 0, 1);
  reverb1.damping(reverb_damping_val);

  reverb_mix_val = analog_map(reverb_mix_pot, 0, 1);
  main_signal_val = 1 - reverb_mix_val;
  reverb_mix.gain(0, main_signal_val); // Original
  reverb_mix.gain(1, reverb_mix_val); // Reverb
  Serial.println("reverb value " + String(reverb_mix_val));

  /*  VOLUME AND BYPASS SECTION  */
  master_volume_val = analog_map(master_volume_pot, 0, 1);
  amp1.gain(master_volume_val);
  Serial.println("master volume " + String(master_volume_val));

  // Check if bypass mode is on
  bypass_switch_state = digitalRead(1);
  if(bypass_switch_state == LOW)
  {
    bypass_mix.gain(0, 1); // Clean signal
    bypass_mix.gain(1, 0); // Effects signal
  }
  else
  {
    bypass_mix.gain(0, 0); // Clean signal
    bypass_mix.gain(1, 1); // Effects signal
  }
}

// Map analog value from pot to digital value
// Returns mapped digital float value
float analog_map(int pot, float range_low, float range_high)
{
  float value = analogRead(pot);
  // value = map(value, 1023*0.1, 1023*.9, 0, 1023);
  float result =  (value - 0.0)/(1023.0 - 0.0)*(range_high-range_low)+range_low;
  return result;
}
