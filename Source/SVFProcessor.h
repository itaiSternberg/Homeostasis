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


class dspSVFProcessor : public dsp::ProcessorBase
{
public:
    
    dspSVFProcessor(AudioProcessorValueTreeState& apvts , int slotIndex)
    : apvts(apvts)
    , slotIndex(String(slotIndex + 1))
    
    {
        
    }
    
    static std::unique_ptr<AudioProcessorParameterGroup> makeParamGroup (String slotIndex)
    {
        return std::make_unique<AudioProcessorParameterGroup> ("filter" + slotIndex,
        "Filter" + slotIndex,
        "|",
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
                                                  "hz",
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
    }

    void prepare (const juce::dsp::ProcessSpec& spec) override
    {
        mStateVariableFilter.prepare(spec);

    }
    
    void process (const juce::dsp::ProcessContextReplacing<float>& context) override
    {
        updateFilter();
        mStateVariableFilter.process(context);

    }
    
    void reset () override
    {
        mStateVariableFilter.reset ();
    }
private:
    
    AudioProcessorValueTreeState& apvts;
    String slotIndex;

    dsp::StateVariableTPTFilter<float> mStateVariableFilter;
    void updateFilter ()
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
};


