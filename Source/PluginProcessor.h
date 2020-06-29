/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SVFProcessor.h"


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
    void initialiseFeedbackGraph ();
    void setNodesConfig (Node::Ptr node);
    std::vector<std::unique_ptr<RangedAudioParameter>> mParameters;
   
    void setSlotNode(int index, std::unique_ptr<AudioProcessor> processor);
    void makeSlotConnections ();
    void parameterChanged(const String &parameterID, float newValue) override;
    
    std::unique_ptr<AudioProcessorGraph> feedbackProcessor; // this is the feedback loop chain
    Node::Ptr feedbackInputNode;                            // this is the feedback loop input
    Node::Ptr feedbackOutpoutNode;                          // this is the feedback loop Output
    
    Node::Ptr feedbackNode1;
    Node::Ptr feedbackNode2;
    Node::Ptr feedbackNode3;
    Node::Ptr feedbackNode4;
    
    int mNumberOfSlots {4};
    
    std::atomic <bool> shouldUpdate {false};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HomeostasisAudioProcessor)
};



