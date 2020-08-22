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

template <typename Type>
class ProcessorChain
{
public:
  
    void prepareToPlay (double sampleRate, int samplesPerBlock, int numChannels)
    {
        mNumChannels = numChannels;
        mSampleRate = sampleRate;
        maxBlockSize = samplesPerBlock;
        
        for (auto& processor : dynamicProcessors)
        {
            processor.get()->prepareToPlay(mSampleRate,maxBlockSize);
        }
        initialiseChain();
    }
    
    void processBlock (AudioBuffer<Type>& buffer, MidiBuffer& midiMessages)
    {
        juce::dsp::AudioBlock<Type> block;

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
    

    template <class T>
    void repplaceProcessor (int index)
    {
        dynamicProcessors[index].reset (new T);
        setProcessorConfig(dynamicProcessors[index]);
        dynamicProcessors[index]->prepareToPlay (mSampleRate, maxBlockSize);

    }
    
    template <class T>
       void repplaceProcessor (AudioProcessorValueTreeState& tree ,int index)
       {
           dynamicProcessors[index].reset (new T (tree, index));
           setProcessorConfig(dynamicProcessors[index]);
           dynamicProcessors[index]->prepareToPlay (mSampleRate, maxBlockSize);
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
    
//    dsp::processS
private:
    std::vector<std::unique_ptr<ProcessorBase>> dynamicProcessors;
  
    uint32 maxBlockSize;
    uint32 mNumChannels;
    int mSampleRate;
    
};
