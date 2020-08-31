/*
 ==============================================================================
 
 feedbackProcessor.h
 Created: 19 Jul 2020 5:08:29pm
 Author:  Itai Sternberg
 
 ==============================================================================
 */

#pragma once
#include <JuceHeader.h>
#include "SVFProcessor.h"
#include "DistortionProcessor.h"
#include "PhaserProcessor.h"
#include "DelayLine.h"

template <typename Type>
class ProcessorChain
{
public:
    void prepareToPlay (double sampleRate, int samplesPerBlock, int numChannels)
    {
        mNumChannels = numChannels;
        mSampleRate = sampleRate;
        maxBlockSize = samplesPerBlock;
        
        spec.sampleRate = sampleRate;
        spec.maximumBlockSize = samplesPerBlock;
        spec.numChannels = numChannels;
        
        for (auto& processor : dynamicProcessors)
        {
            processor.get()->prepareToPlay(mSampleRate,maxBlockSize);
        }
        initialiseChain();
    }
    
    void processBlock (AudioBuffer<Type>& buffer, MidiBuffer& midiMessages)
    {
        block = juce::dsp::AudioBlock<float> (buffer);
        
        for (auto& processor : dynamicProcessors)
        {
            processor.get()->processBlock(buffer, midiMessages);
        }
    }
    
    void releaseResources()
    {
        for (auto& processor : dynamicProcessors)
        {
            processor.get()->releaseResources();
        }
    }
    
    void initialiseChain () // helper function which initialises the processing chains
    {
        for (size_t i = 0; i < 4; ++i)
        {
            dynamicProcessors.emplace_back (std::make_unique<ProcessorBase> ());
        }
        
        for (auto& processor : dynamicProcessors)
        {
            setProcessorConfig(processor);
        }
    }
    
    void setProcessorConfig (std::unique_ptr<ProcessorBase>& processor)
    {
        processor.get()->setPlayConfigDetails(mNumChannels, mNumChannels, mSampleRate, maxBlockSize);
    }
    

    template <class T, typename... Args>
    void replaceProcessor (int index, Args&&... args)
    {
        auto* p = new T (std::forward<Args>(args)...);
        p->setPlayConfigDetails(mNumChannels, mNumChannels, mSampleRate, maxBlockSize);
        dynamicProcessors[index].reset(p);
    }
    
    void processorChanged (String choice,int paramIndex, AudioProcessorValueTreeState& tree)         /* Check which processor chosen and instantiate                                                                                                  it.*/
    {
        if (choice == "Empty")
        {
            replaceProcessor<ProcessorBase> (paramIndex);
        }
        else if (choice == "Filter")
        {
            replaceProcessor<SVFProcessor> (paramIndex, tree, paramIndex, spec, block, mNumChannels);
        }
        else if (choice == "Phaser")
        {
            replaceProcessor<PhaserProcessor> (paramIndex, tree, paramIndex, spec, block, mNumChannels);
        }
        else if (choice == "Distortion")
        {
            replaceProcessor<DistortionProcessor> (paramIndex, tree, paramIndex);
        }
    }
    juce::dsp::ProcessSpec spec;
    juce::dsp::AudioBlock<Type> block;
private:
    std::vector<std::unique_ptr<ProcessorBase>> dynamicProcessors;
  
    uint32 maxBlockSize;
    uint32 mNumChannels;
    int mSampleRate;
};
