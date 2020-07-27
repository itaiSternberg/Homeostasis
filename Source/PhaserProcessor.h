/*
  ==============================================================================

    PhaserProcessor.h
    Created: 26 Jul 2020 10:04:26am
    Author:  Itai Sternberg

  ==============================================================================
*/

#pragma once
#include "ProcessorBase.h"
class PhaserProcessor : public ProcessorBase
{
public:
    PhaserProcessor(AudioProcessorValueTreeState& apvts, int slotIndex);
    const String getName() const override {return "Phaser";};
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    static std::unique_ptr<AudioProcessorParameterGroup> makeParamGroup (String slotIndex);
    void updatePhaser();
private:
    
    AudioProcessorValueTreeState& apvts;
    dsp::Phaser<float> phaser;
    String slotIndex;
};
