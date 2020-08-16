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
#include "Wavetable.h"
#include "PhaserProcessor.h"
#include "DelayLine.h"

class ProcessorChain
{
public:
  
    void prepareToPlay (double sampleRate, int samplesPerBlock)
    {
        mSampleRate = sampleRate;
        maxBlockSize = samplesPerBlock;
        
        for (auto& processor : dynamicProcessors)
        {
            processor.get()->prepareToPlay(mSampleRate,maxBlockSize);
        }
        initialiseChain();
    }
    
    void processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
    {
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
        processor.get()->setPlayConfigDetails(numChannels, numChannels, mSampleRate, maxBlockSize);
    }
    

    template <class T>
    void repplaceProcessor (int index)
    {
        jassert (index >= 0 && index <= 3);
        dynamicProcessors[index].reset (new T);
    }
    
    template <class T>
       void repplaceProcessor (AudioProcessorValueTreeState& tree ,int index)
       {
           jassert (index >= 0 && index <= 3);
           dynamicProcessors[index].reset (new T (tree, index));
       }
    
    
    
    void processorChanged (String choice,int paramIndex, AudioProcessorValueTreeState& tree)         //========================== Check which processor chosen and instantiate it.
    {
        if (choice == "Empty")
        {
            repplaceProcessor<ProcessorBase> (paramIndex);
        }
        else if (choice == "Filter")
        {
            repplaceProcessor<SVFProcessor> (tree, paramIndex);
        }
        else if (choice == "Phaser")
        {
            repplaceProcessor<PhaserProcessor> (tree, paramIndex);
        }
        else if (choice == "Distortion")
        {
            repplaceProcessor<DistortionProcessor> (tree, paramIndex);
        }
        
        
        for (auto& processor : dynamicProcessors)
        {
            setProcessorConfig(processor);
            processor.get()->prepareToPlay(mSampleRate, maxBlockSize);
            processor.get()->enableAllBuses();
        }
    }
private:
    std::vector<std::unique_ptr<ProcessorBase>> dynamicProcessors;
  
    uint32 maxBlockSize;
    uint32 numChannels {2};
    int mSampleRate;
    
};
