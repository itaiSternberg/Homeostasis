/*
  ==============================================================================

    DistortionProcessor.h
    Created: 10 Jul 2020 12:45:51am
    Author:  Itai Sternberg

  ==============================================================================
*/

#pragma once
#include "ProcessorBase.h"

class DistortionProcessor : public ProcessorBase
{
public:
    DistortionProcessor(AudioProcessorValueTreeState& apvts , int slotIndex);
    ~DistortionProcessor();
    const String getName() const override {return "Distortion";};
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    static std::unique_ptr<AudioProcessorParameterGroup> makeParamGroup (String slotIndex);

private:
    
    AudioProcessorValueTreeState& apvts;
    enum {
        preGainIndex,
        WaveshaperIndex,
        postGainIndex
    };
    
    String slotIndex;
    juce::dsp::ProcessorChain<juce::dsp::Gain<float>,juce::dsp::WaveShaper<float>,juce::dsp::Gain<float>> processorChain;
};
