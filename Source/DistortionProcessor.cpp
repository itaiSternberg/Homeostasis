/*
  ==============================================================================

    DistortionProcessor.cpp
    Created: 10 Jul 2020 12:45:51am
    Author:  Itai Sternberg

  ==============================================================================
*/

#include "DistortionProcessor.h"

DistortionProcessor::DistortionProcessor (AudioProcessorValueTreeState& apvts , int slotIndex)
: apvts(apvts)
, slotIndex (String(slotIndex + 1))
{
    auto& waveshaper = processorChain.template get<WaveshaperIndex>();
    waveshaper.functionToUse = [] (float x)
                                {
                                    return std::tanh(x);
                                };
    
    auto& preGain = processorChain.template get<preGainIndex>();
    preGain.setGainDecibels(30.0f);
    
    auto& postGain = processorChain.template get<postGainIndex>();
    postGain.setGainDecibels(-20.0f);
    
}

DistortionProcessor::~DistortionProcessor ()
{
    
}


void DistortionProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;

    processorChain.reset();
    processorChain.prepare(spec);
}
void DistortionProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    dsp::AudioBlock<float> block (buffer);
    processorChain.process(dsp::ProcessContextReplacing<float> (block));
}
std::unique_ptr<AudioProcessorParameterGroup> DistortionProcessor::makeParamGroup (String slotIndex)
{
    
}




