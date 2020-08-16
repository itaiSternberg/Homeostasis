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
    updateGain();
    processorChain.process(dsp::ProcessContextReplacing<float> (block));
}
std::unique_ptr<AudioProcessorParameterGroup> DistortionProcessor::makeParamGroup (String slotIndex)
{

    return std::make_unique<AudioProcessorParameterGroup> ("distortion" + slotIndex,
                                                           "Distortion " + slotIndex,
                                                           "|",
                                                           std::make_unique<AudioParameterFloat>("drive" + slotIndex,
                                                                                                 "Drive" + slotIndex,
                                                                                                 NormalisableRange<float> (0.0f,
                                                                                                                           50.0f,
                                                                                                                              0.1f,
                                                                                                                              1
                                                                                                                              )
                                                                                                 ,
                                                                                                 0.0f,
                                                                                                 "dB",
                                                                                                 AudioProcessorParameter::genericParameter,
                                                                                                 nullptr,
                                                                                                 nullptr
                                                                                                 ),
                                                           std::make_unique<AudioParameterFloat>("distortionGain" + slotIndex,
                                                           "Distortion Gain " + slotIndex,
                                                           NormalisableRange<float> (-100.0f,
                                                                                     0.0f,
                                                                                        0.1f,
                                                                                        1
                                                                                        )
                                                           ,
                                                           0.0f,
                                                           "dB",
                                                           AudioProcessorParameter::genericParameter,
                                                           nullptr,
                                                           nullptr
                                                           )
                                                           );
}





void DistortionProcessor::updateGain()
{
    float drive = *apvts.getRawParameterValue("drive" + slotIndex);
    float gain = *apvts.getRawParameterValue("distortionGain" + slotIndex);
    auto& preGain = processorChain.template get<preGainIndex>();
    preGain.setGainDecibels(drive);
    
    auto& postGain = processorChain.template get<postGainIndex>();
    postGain.setGainDecibels(gain);
}
