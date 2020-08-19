/*
 ==============================================================================
 
 Wavetable.cpp
 Created: 11 Jul 2020 4:40:27pm
 Author:  Itai Sternberg
 
 ==============================================================================
 */

#include "Wavetable.h"
Wavetable::Wavetable ()
{
    DBG(getBusesLayout().getChannelSet(true, 0).getDescription());
    delayLineSize = 10;
    delayLine.resize(delayLineSize);
    synth.addVoice(new SynthVoice());
    synth.addSound(new SynthSound());
}


Wavetable::~Wavetable()
{
}


void Wavetable::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    lastSampleRate = sampleRate;
}


void Wavetable::processBlock (AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    
    synth.renderNextBlock(buffer, midiMessages, 0, getTotalNumOutputChannels());
}
void Wavetable::reset()
{
    
}


void Wavetable::resizeDelayLineToFreq (DelayLine<float> &delayLine, int lastSampleRate, const juce::MidiMessageMetadata &metadata)
{
    unsigned int frequency = juce::MidiMessage::getMidiNoteInHertz(metadata.getMessage().getNoteNumber());
    delayLineSize = (size_t) juce::roundToInt (lastSampleRate / frequency);
    delayLine.resize(delayLineSize);
}


