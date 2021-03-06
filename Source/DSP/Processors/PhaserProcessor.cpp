/*
  ==============================================================================

    PhaserProcessor.cpp
    Created: 26 Jul 2020 10:04:26am
    Author:  Itai Sternberg

  ==============================================================================
*/

#include "PhaserProcessor.h"
PhaserProcessor::PhaserProcessor (AudioProcessorValueTreeState& apvts , int slotIndex, dsp::ProcessSpec& spec, dsp::AudioBlock<float>& block, const int numChannels)
: apvts(apvts)
, slotIndex(String(slotIndex + 1))
, mSpec (spec)
, mBlock(block)

{
    phaser.reset();
}
void PhaserProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    phaser.prepare(mSpec);
}

void PhaserProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    updatePhaser();
    phaser.process(dsp::ProcessContextReplacing <float> (mBlock));
}

void PhaserProcessor::releaseResources()
{
}
std::unique_ptr<AudioProcessorParameterGroup> PhaserProcessor::makeParamGroup (String slotIndex)
{
    return std::make_unique<AudioProcessorParameterGroup> (
                                                           "phaser" + slotIndex,
                                                           "Phaser" + slotIndex,
                                                           "|",
                                                           std::make_unique<AudioParameterFloat> ("phaserRate" + slotIndex ,
                                                                                                  "Phaser Rate" + slotIndex,
                                                                                                  NormalisableRange<float>(
                                                                                                                           0.0,
                                                                                                                           20.0f,
                                                                                                                           0.01f,
                                                                                                                           0.2,
                                                                                                                           false
                                                                                                                           ),
                                                                                                  0.02f,
                                                                                                  "Hz",
                                                                                                  AudioProcessorParameter::genericParameter,
                                                                                                  nullptr,
                                                                                                  nullptr),
                                                                                                  
                                                           std::make_unique<AudioParameterFloat> ("phaserDepth" + slotIndex ,
                                                                                                  "Phaser Depth" + slotIndex,
                                                                                                  0.0f,
                                                                                                  1.0f,
                                                                                                  0.2),
                                                           
                                                           std::make_unique<AudioParameterFloat> ("phaserFeedback" + slotIndex ,
                                                                                                  "Phaser Feedback" + slotIndex,
                                                                                                                           -0.99f,
                                                                                                                           0.99f,
                                                                                                                           0.0f
                                                                                                  ),
                                                           
                                                           std::make_unique<AudioParameterFloat> ("phaserMix" + slotIndex ,
                                                                                                  "Phaser Mix" + slotIndex,
                                                                                                  0.0f,
                                                                                                  0.5f,
                                                                                                  0.5f)
                                                           

                                                           );
}


void PhaserProcessor::updatePhaser()
{
    float mix = *apvts.getRawParameterValue("phaserMix" + slotIndex);
    float rate = *apvts.getRawParameterValue("phaserRate" + slotIndex);
    float depth = *apvts.getRawParameterValue("phaserDepth" + slotIndex);
    float feedback = *apvts.getRawParameterValue("phaserFeedback" + slotIndex);
    
    phaser.setMix(mix);
    phaser.setRate(rate);
    phaser.setDepth(depth);
    phaser.setFeedback(feedback);
}
