/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "FeedbackGraph.h"


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
 //==============================================================================
    std::vector<std::unique_ptr<RangedAudioParameter>> mParameters;
    Wavetable processor0;
    
    CircularBuffer<float, 1> circularBuffer;
    void parameterChanged(const String &parameterID, float newValue) override;
    
    void midiHandeling (MidiBuffer& midiMessages);
    void clearDelayBufferIfNewNote (MidiMessageMetadata metadata);
    void setDelayTImeToFreq (MidiMessageMetadata metadata);
    
    std::atomic<bool> newNote {false};
    
    ProcessorChain<float> feedbackChain;
    ProcessorChain<float> masterChain;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HomeostasisAudioProcessor)
};



