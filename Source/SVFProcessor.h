/*
  ==============================================================================

    SVFProcessor.h
    Created: 17 Jun 2020 1:34:43pm
    Author:  Itai Sternberg

  ==============================================================================
*/

#pragma once
#include "ProcessorBase.h"

class SVFProcessor : public ProcessorBase
{
public:
    SVFProcessor(AudioProcessorValueTreeState& apvts , int slotIndex, dsp::ProcessSpec& spec, dsp::AudioBlock<float>& block, const int numChannels);
    ~SVFProcessor();
    const String getName() const override {return "Filter";};
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    
    static std::unique_ptr<AudioProcessorParameterGroup> makeParamGroup (String slotIndex);
private:
    juce::dsp::ProcessSpec& mSpec;
    juce::dsp::AudioBlock<float>& mBlock;

    AudioProcessorValueTreeState& apvts;
    String slotIndex;
    float lastSampleRate;
    void updateFilter ();
    dsp::StateVariableTPTFilter<float> mStateVariableFilter;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SVFProcessor)
};


