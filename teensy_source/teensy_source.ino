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
AudioEffectWaveshaper    waveshape1;     //xy=455.33331298828125,487.3333435058594
AudioMixer4              waveshape_mix;         //xy=617.3333129882812,364.3333435058594
AudioEffectDelay         delay1;         //xy=737,486.33331298828125
AudioMixer4              delay_mix;         //xy=858.3333129882812,353.33331298828125
AudioEffectFreeverb      reverb1;      //xy=1017.333251953125,470.3333435058594
AudioMixer4              reverb_mix;         //xy=1169.6666259765625,366.33331298828125
AudioOutputI2S           i2s_out;           //xy=1303.33349609375,368.3333435058594
AudioConnection          patchCord1(i2s_in, 0, pan_mix, 0);
AudioConnection          patchCord2(i2s_in, 1, pan_mix, 1);
AudioConnection          patchCord3(pan_mix, 0, filter1, 0);
AudioConnection          patchCord4(filter1, 0, filter_mix, 0);
AudioConnection          patchCord5(filter1, 1, filter_mix, 1);
AudioConnection          patchCord6(filter1, 2, filter_mix, 2);
AudioConnection          patchCord7(filter_mix, waveshape1);
AudioConnection          patchCord8(filter_mix, 0, waveshape_mix, 0);
AudioConnection          patchCord9(waveshape1, 0, waveshape_mix, 1);
AudioConnection          patchCord10(waveshape_mix, delay1);
AudioConnection          patchCord11(waveshape_mix, 0, delay_mix, 0);
AudioConnection          patchCord12(delay1, 0, delay_mix, 1);
AudioConnection          patchCord13(delay_mix, reverb1);
AudioConnection          patchCord14(delay_mix, 0, reverb_mix, 0);
AudioConnection          patchCord15(reverb1, 0, reverb_mix, 1);
AudioConnection          patchCord16(reverb_mix, 0, i2s_out, 0);
AudioConnection          patchCord17(reverb_mix, 0, i2s_out, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=244.3333282470703,597.3333282470703
// GUItool: end automatically generated code

// Define Analog Pins
// Free A0, A2, A3, A8, A10-1, A11-1, A12-2, A13-2, A14-2, A15-2, A16, A17
// Free Digital pins 0, 1, 2, 3, 4, 5
#define headphone_volume_pot A1
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

// Variables
float cutoff_freq_val = 15000; // 0 to 15000 Hz
float waveshape_mix_val = 0; // 0 to 1
float delay_ms_val = 100; // 0 to 1000 ms
float delay_mix_val = 0; // 0 to 1
float reverb_roomsize_val = 0; // 0 to 1
float reverb_damping_val = 0; // 0 to 1
float reverb_mix_val = 0; // 0 to 1
float main_signal_val = 0; // 0 to 1
float headphone_volume_val = 0; // 0 to 1

void setup() 
{
  Serial.begin(9600);
  pinMode(0, INPUT_PULLUP); // Filter choice button
  AudioMemory(1500);
  sgtl5000_1.enable(); // Start SGTL5000
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN); // Select line in input
  sgtl5000_1.volume(1); // Set headphone volume level

  // Does not change, both left and right channels are balanced
  pan_mix.gain(0, 0.5); // Left
  pan_mix.gain(1, 0.5); // Right

  filter1.frequency(cutoff_freq_val);

  filter_mix.gain(0, 1); // Low pass Output
  filter_mix.gain(1, 0); // Band Pass Output
  filter_mix.gain(2, 0); // High Pass Output

  delay1.delay(0, delay_ms_val);

  delay_mix.gain(0, 1); // Original signal
  delay_mix.gain(0, 0); // Delay mix

  reverb1.roomsize(reverb_roomsize_val);
  reverb1.damping(reverb_damping_val);

  reverb_mix.gain(0, 1); // Original signal
  reverb_mix.gain(1, 0); // Reverb Mix
}

void loop() 
{
   
  /*  FREQUENCY SECTION   */
  cutoff_freq_val = analog_map(cutoff_pot, 0, 15000);
  filter1.frequency(cutoff_freq_val);

  /*  WAVESHAPE SECTION   */
  waveshape_mix_val = analog_map(waveshape_mix_pot, 0, 1);
  main_signal_val = 1 - waveshape_mix_val;
  waveshape_mix.gain(0, main_signal_val); // Original
  waveshape_mix.gain(1, waveshape_mix_val); // Waveshape

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

  /*  VOLUME SECTION  */
  //headphone_volume_val = analog_map(headphone_volume_pot, 0, 1);
  //sgtl5000_1.volume(headphone_volume_val);
  
  /* PROCESSOR USAGE  */
  Serial.print("Audio Processor Usage Max: ");
  Serial.println(AudioProcessorUsageMax());
 
}

// Map analog value from pot to digital value
// Returns mapped digital value
float analog_map(int pot, int range_low, int range_high)
{
  int value = analogRead(pot); 
  return map(value, 0, 1023, range_low, range_high);
}
