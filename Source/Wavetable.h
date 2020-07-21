/*
  ==============================================================================

    Wavetable.h
    Created: 11 Jul 2020 4:40:27pm
    Author:  Itai Sternberg

  ==============================================================================
*/
#pragma once
#include <JuceHeader.h>
#include "ProcessorBase.h"
class Wavetable : public ProcessorBase
{
public:
    Wavetable();
    ~Wavetable();
    const juce::String getName() const override { return "Source"; }
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer&) override;
    void reset() override;

private:
    Random random;
    enum
    {
        oscIndex,
        gainIndex
    };
    dsp::ProcessorChain<dsp::Oscillator<float>,dsp::Gain<float>> chain;
    dsp::Oscillator<float>& randomTable;
};
#pragma once
