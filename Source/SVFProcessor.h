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
    SVFProcessor(AudioProcessorValueTreeState& apvts , int slotIndex);
    ~SVFProcessor();
    const String getName() const override {return "Filter";};
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    
    static std::unique_ptr<AudioProcessorParameterGroup> makeParamGroup (String slotIndex);
private:
    AudioProcessorValueTreeState& apvts;
    String slotIndex;
    float lastSampleRate;
    dsp::StateVariableTPTFilter<float> mStateVariableFilter;

    void updateFilter ();

};
