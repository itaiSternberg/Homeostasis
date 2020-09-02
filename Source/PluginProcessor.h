/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "DSP/Utils/ProcessorChain.h"
#include "DSP/Utils/DelayLine.h"


//==============================================================================
/**
*/
class HomeostasisAudioProcessor  : public AudioProcessor,
                                   public ValueTree::Listener,
                                   public AudioProcessorValueTreeState::Listener
{
public:
    using AudioGraphIOProcessor = AudioProcessorGraph::AudioGraphIOProcessor;
    using Node = AudioProcessorGraph::AudioProcessorGraph::Node;
    //==============================================================================
    HomeostasisAudioProcessor();
    ~HomeostasisAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    //==============================================================================
    
    AudioProcessorValueTreeState::ParameterLayout MainTreeLayout ();
    StringArray processorChoises;
    AudioProcessorValueTreeState mainTree;

private:
    void parameterChanged(const String &parameterID, float newValue) override;
    
    void changeProcessors (String choice, int paramIndex);
    std::atomic<bool> shouldProcessorChange;
    String processorThatChanged;
    int processorIndex;
    
    void midiHandeling (MidiBuffer& midiMessages);
    void setDelayTimeToFreq (MidiMessage message);

    std::vector<std::unique_ptr<RangedAudioParameter>> mParameters;
    
    double delayTimeInSamples;
    CircularBuffer<float> circularBufferL;
    CircularBuffer<float> circularBufferR;
    
    ProcessorChain<float> feedbackChainL;
    ProcessorChain<float> feedbackChainR;
   
    ProcessorChain<float> masterChain;
    
    
    dsp::Limiter<float> limiter;
    dsp::Gain<float> gain;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HomeostasisAudioProcessor)
};



