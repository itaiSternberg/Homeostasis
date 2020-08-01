/*
  ==============================================================================

    DelayLine.h
    Created: 18 Jul 2020 11:10:24am
    Author:  Itai Sternberg

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

template <typename Type>
class DelayLine
{
public:
    void clear() noexcept
    {
        std::fill (rawData.begin(), rawData.end(), Type (0));
    }
    
    size_t size() const noexcept
    {
        return rawData.size();
    }
    
    void resize(size_t newValue)  
    {
        rawData.resize(newValue);
        leastRecentIndex = 0;
    }
    
    Type back() const noexcept
    {
        return rawData[leastRecentIndex];
    }
    
    void push(Type valueToAdd) noexcept
    {
        rawData[leastRecentIndex] = valueToAdd;
        leastRecentIndex = leastRecentIndex == 0 ? size() - 1 : leastRecentIndex - 1;
    }
    
    Type get(size_t delayInSamples) const noexcept
    {
        jassert(delayInSamples >= 0 && delayInSamples <= size());
        return rawData[(leastRecentIndex + 1 + delayInSamples) % size()];
    }
    
    void set(size_t delayInSamples, Type valueToAdd) noexcept
    {
        jassert(delayInSamples >= 0 && delayInSamples <= size());
        rawData[(leastRecentIndex + 1 + delayInSamples) % size()] = valueToAdd;
    }
        
private:
    std::vector<Type> rawData;
    size_t leastRecentIndex {0};
};

template <typename Type, size_t maxNumChannels = 2>
class Delay
{
public:
    Delay()
    {
        constexpr int left = 0;
        constexpr int right = 0;
        
        setMaxDelayTime(100);
        setDelayTime(25,left);
        setDelayTime(25,right);
    }
    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        jassert (spec.numChannels <= maxNumChannels);
        sampleRate = spec.sampleRate;
        updateDelayLineSize();
        updateDelayTime();
    }

    void reset() noexcept
    {
        for (auto& delayLine : delayLines)
        {
            delayLine.clear();
        }
    }

    size_t getNumChannels () const noexcept
    {
        return delayLines.size();
    }



    void setMaxDelayTime(Type newValue)
    {
        jassert(newValue > Type (0));
        maxDelayTime = newValue;
        updateDelayLineSize();

    }

    void setDelayTime (Type newValue, size_t channel)
    {
        if (channel >= getNumChannels())
        {
            jassertfalse;
            return;
        }

        jassert (newValue > Type (0));
        delayTimes[channel] = newValue;
    }

    template<typename ProcessContext>
    void process(const ProcessContext& context)
    {
        auto& inputBlock = context.getInputBlock();
        auto& outputBlock = context.getOutputBlock();
        auto numSamples = outputBlock.getNumSamples();
        auto numChannels = outputBlock.getNumChannels();

        jassert(inputBlock.getNumSamples() == numSamples);
        jassert(inputBlock.getNumChannels() == numChannels);
        for (size_t channel = 0; channel < numChannels; ++channel)
        {
            auto* input = inputBlock.getChannelPointer(channel);
            auto* output = outputBlock.getChannelPointer(channel);
            auto& delayLine = delayLines[channel];
            auto delayTime = delayTimes[channel];

            for (size_t sample = 0 ; sample < numSamples; ++sample)
            {
                auto delayedSample = delayLine.get(delayTime);
                auto inputSample = input[sample];
                auto delayLineInputSample = std::tanh (inputSample + delayedSample);
                delayLine.push (delayLineInputSample);
                auto outputSample = delayedSample;
                output[sample] = outputSample;
            }
        }

    }

private:
    std::array<DelayLine<Type>, maxNumChannels> delayLines;
    std::array <Type, maxNumChannels> delayTimes;
    std::array <size_t, maxNumChannels> delayTimesInSamples;
    Type sampleRate {Type (44100)};
    Type maxDelayTime {Type (0)};

    void updateDelayLineSize ()
    {
        auto delayLineSizeSamples = std::ceil (maxDelayTime * sampleRate);
        for (auto& delayLine : delayLines)
        {
            delayLine.resize(delayLineSizeSamples);
        }
    }

    void updateDelayTime() noexcept
    {
        for (size_t channel = 0; channel < maxNumChannels; ++channel)
        {
            delayTimesInSamples[channel] = (size_t) juce::roundToInt(delayTimes[channel] * sampleRate);
        }
    }

};
