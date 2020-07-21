/*
  ==============================================================================

    Wavetable.cpp
    Created: 11 Jul 2020 4:40:27pm
    Author:  Itai Sternberg

  ==============================================================================
*/

#include "Wavetable.h"
Wavetable::Wavetable ()
: random()
,randomTable(chain.get<oscIndex>())
{
    
    randomTable.setFrequency(30.0f);
    randomTable.initialise([this] (float x) {if (random.nextInt() % 2 == 0) {return -x;} { return x;} });
    chain.get<gainIndex>().setGainLinear(0.05);
}


Wavetable::~Wavetable()
{
}

void Wavetable::prepareToPlay (double sampleRate, int samplesPerBlock)
    {
        juce::dsp::ProcessSpec spec { sampleRate, static_cast<juce::uint32> (samplesPerBlock) };
        chain.prepare (spec);
    }

void Wavetable::processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer&)
    {
        juce::dsp::AudioBlock<float> block (buffer);
        juce::dsp::ProcessContextReplacing<float> context (block);
        chain.process (context);
    }

void    Wavetable::reset()
    {
        chain.reset();
    }

//void Wavetable::createWavetable()
//{
//    sineTable.setSize (1, (int) tableSize);
//    auto* samples = sineTable.getWritePointer(0);
//    auto angleDelta = juce::MathConstants<double>::twoPi / (double) tableSize / 2;
//    auto currentAngle = 0.0;
//    
//    for (unsigned int i = 0; i < tableSize; ++i)
//    {
//        auto sample = std::sin(currentAngle);
//        samples[i] = (float) sample;
//        currentAngle += angleDelta;
//        
//    }
//    
//}
