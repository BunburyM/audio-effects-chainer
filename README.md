# audio-effects-chainer

A 4-person team project created during the 13th HackUMass event. This repository holds our source code for our multi-effects pedal, with the Teensy 4.1 as our chosen microcontroller.

Our multi-effects pedal takes in an audio input and modifies it with four different effects in the following order: filter, bitcrush, delay, and reverb. The output can then be heard through speakers or headphones. 

For the filtering stage, we use the ladder filter, a famous low-pass filter implementation in synthesis. One can choose the cutoff frequency and the resonance with their respective knobs.

Bitcrushing, the next stage, is a type of distortion. One can reduce the bit depth and sample rate of the incoming signal through knobs.

The 3rd stage, delay, offers a timing parameter in milliseconds to tweak the effect with a knob.

The final stage, reverb, offers room size and damping parameters to tweak with their respective knobs.

If needed, there is functionality for the entire signal path to be bypassed with a switch.

This is the flowchart of the signal path, created through the Audio System Design Tool of the Teensy Audio Library:
![alt text](https://github.com/BunburyM/audio-effects-chainer/blob/main/system_image.png?raw=true)
