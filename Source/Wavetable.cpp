/*
 ==============================================================================
 
 Wavetable.cpp
 Created: 11 Jul 2020 4:40:27pm
 Author:  Itai Sternberg
 
 ==============================================================================
 */

#include "Wavetable.h"
Wavetable::Wavetable ()
: synthAudioSource(keyboardState)
{
}


Wavetable::~Wavetable()
{
}


void Wavetable::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synthAudioSource.prepareToPlay(samplesPerBlock, sampleRate);
}

void Wavetable::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer&)
{
    juce::AudioSourceChannelInfo info (buffer) ;
    synthAudioSource.getNextAudioBlock(info);
}

void Wavetable::reset()
{
    synthAudioSource.releaseResources();

}


