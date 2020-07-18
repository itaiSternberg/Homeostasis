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
