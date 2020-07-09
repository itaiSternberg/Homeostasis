/*
  ==============================================================================

    SVFProcessor.cpp
    Created: 17 Jun 2020 1:34:43pm
    Author:  Itai Sternberg

  ==============================================================================
*/
#include "SVFProcessor.h"

SVFProcessor::SVFProcessor(AudioProcessorValueTreeState& apvts , int slotIndex)
: apvts(apvts)
, slotIndex(String(slotIndex + 1))

{}
SVFProcessor::~SVFProcessor()
{
    
}

std::unique_ptr<AudioProcessorParameterGroup> SVFProcessor::makeParamGroup (String slotIndex)
{
    return std::make_unique<AudioProcessorParameterGroup> ("filter" + slotIndex,
                                        "Filter" + slotIndex,
                                        "seperator",
                                        std::make_unique<AudioParameterFloat>("cutoff" + slotIndex,
                                                                                  "Cutoff Frequency " + slotIndex,
                                                                                  NormalisableRange<float>(
                                                                                                           40.0f,
                                                                                                           18000.0f,
                                                                                                           0.1f,
                                                                                                           0.3,
                                                                                                           false
                                                                                                           ),
                                                                                  500.0f,
                                                                                  String(),
                                                                                  AudioProcessorParameter::genericParameter,
                                                                                  nullptr,
                                                                                  nullptr),

                                        std::make_unique<AudioParameterFloat> ("res" + slotIndex ,
                                                                               "Resonance" + slotIndex,
                                                                               NormalisableRange<float>(
                                                                                                        1.0f,
                                                                                                        5.0f,
                                                                                                        0.1f,
                                                                                                        1,
                                                                                                        false
                                                                                                        ),
                                                                               0.0f,
                                                                               String(),
                                                                               AudioProcessorParameter::genericParameter,
                                                                               nullptr,
                                                                               nullptr),

                                        std::make_unique<AudioParameterChoice> (
                                                                                "filterType" + slotIndex,
                                                                                "Filter Type" + slotIndex,
                                                                                Array<String> {"HP","LP","BP"},
                                                                                0
                                                                                ));
};

void SVFProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    lastSampleRate = sampleRate;

    dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;


    mStateVariableFilter.reset();
    mStateVariableFilter.prepare(spec);
}

void SVFProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
   
//    buffer.setSize(getMainBusNumInputChannels(), buffer.getNumSamples());

    dsp::AudioBlock<float> block (buffer);
    updateFilter();
    mStateVariableFilter.process(dsp::ProcessContextReplacing<float> (block));
}

void SVFProcessor::updateFilter ()
{
    float resonance = *apvts.getRawParameterValue("res" + slotIndex);
    float freq = *apvts.getRawParameterValue("cutoff"  + slotIndex);
    int choice = *apvts.getRawParameterValue("filterType" + slotIndex);

    if (choice == 0)
    {
        mStateVariableFilter.setType(dsp::StateVariableTPTFilterType::highpass);
        mStateVariableFilter.setCutoffFrequency(freq);
        mStateVariableFilter.setResonance(resonance);

    } else if (choice == 1)

    {
       mStateVariableFilter.setType(dsp::StateVariableTPTFilterType::lowpass);
        mStateVariableFilter.setCutoffFrequency(freq);
        mStateVariableFilter.setResonance(resonance);
    } else
    {
        mStateVariableFilter.setType(dsp::StateVariableTPTFilterType::bandpass);
               mStateVariableFilter.setCutoffFrequency(freq);
               mStateVariableFilter.setResonance(resonance);
    }

}



