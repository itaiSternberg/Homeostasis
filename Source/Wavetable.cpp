/*
 ==============================================================================
 
 Wavetable.cpp
 Created: 11 Jul 2020 4:40:27pm
 Author:  Itai Sternberg
 
 ==============================================================================
 */

#include "Wavetable.h"
Wavetable::Wavetable ()
//: keyboardState (keyState)
{
    synth.addVoice(new SynthVoice());
    synth.addSound(new SynthSound());
}


Wavetable::~Wavetable()
{
}


void Wavetable::prepareToPlay (double sampleRate, int samplesPerBlock)
{
            synth.setCurrentPlaybackSampleRate(sampleRate);
}

void Wavetable::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiMessages)
{
//    if (midiMessages.getNumEvents() > 0)
//        DBG(midiMessages.getNumEvents());
//    juce::AudioSourceChannelInfo info (buffer) ;
//    keyboardState.processNextMidiBuffer(midiMessages, info.startSample, info.numSamples, true);
    
    buffer.clear();
    
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

void Wavetable::reset()
{

}


