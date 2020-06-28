/*
  ==============================================================================

    SVFProcessor.h
    Created: 17 Jun 2020 1:34:43pm
    Author:  Itai Sternberg

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#include "ProcessorBase.h"


class SVFProcessor : public ProcessorBase
{
public:
    SVFProcessor();
    ~SVFProcessor();
    const String getName() const override {return "Filter";};
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    
    
    AudioProcessorValueTreeState filterParameters;
private:
    float lastSampleRate;
       dsp::ProcessorDuplicator<dsp::StateVariableFilter::Filter<float>, dsp::StateVariableFilter::Parameters<float>> mStateVariableFilter;

    void updateFilter ();

};
