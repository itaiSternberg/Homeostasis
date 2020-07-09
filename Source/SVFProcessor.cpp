/*
  ==============================================================================

    SVFProcessor.cpp
    Created: 17 Jun 2020 1:34:43pm
    Author:  Itai Sternberg

  ==============================================================================
*/
#include "SVFProcessor.h"

SVFProcessor::SVFProcessor()
: filterParameters(*this, nullptr, Identifier("Parameters"),
           {
        std::make_unique<AudioParameterFloat>("cutoff",
                                              "Cutoff Frequency",
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

    std::make_unique<AudioParameterFloat> ("res",
                                           "Resonance",
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
                                            "filterType",
                                            "Filter Type",
                                            Array<String> {"HP","LP","BP"},
                                            0
                                            )
})

{}
SVFProcessor::~SVFProcessor()
{
    
}


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
    float resonance = *filterParameters.getRawParameterValue("res");
    float freq = *filterParameters.getRawParameterValue("cutoff");
    int choice = *filterParameters.getRawParameterValue("filterType");

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
