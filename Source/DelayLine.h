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


template <typename Type, size_t numChannels = 2>
class CircularBuffer
{
public:
    void makeBuffer (unsigned int bufferLength)
    {
        mBufferLength = (unsigned int) (pow (2, ceil (log (bufferLength) / log(2))));
        wrapMask = mBufferLength - 1;
        buffer.setSize (numChannels, mBufferLength);
        DBG(numChannels);
        clearBuffer ();
    }
    void clearBuffer ()
    {
        buffer.clear();
    }
    
    void writeBuffer (Type newValue, unsigned int channel)
    {
        jassert (channel >= 0 && channel <= buffer.getNumChannels());
        
        auto* writePointer = buffer.getWritePointer( channel);
        writePointer[writeIndex++] = newValue;  // writes the newsample to the position of the writeindex then increments it
        writeIndex &= wrapMask;
        
        // wraps around the size of the buffer
    }
    
    Type readBuffer (int delayInSamples, unsigned int channel)
    {
        jassert (channel >= 0 && channel <= buffer.getNumChannels());

        const auto* readPointer = buffer.getReadPointer (channel);
        int readIndex = writeIndex - delayInSamples;
        readIndex &= wrapMask;            // wraps the readIndex
        return readPointer [readIndex];         // returns the sample at the buffer
    }
    
    Type readBuffer (double delayInFractionalSamples, bool interpolate, unsigned int channel)
    {
        Type y1 =  readBuffer ((int) delayInFractionalSamples, channel);
        if (!interpolate) return y1;
        
        Type y2 = (int) readBuffer ((int) (delayInFractionalSamples + 1) , channel);
        
        double frac = delayInFractionalSamples - (int) delayInFractionalSamples;
        
        return linearInterpolation (y1, y2, frac);
    }
    
    
private:
   static double linearInterpolation (double y1, double y2, double fractional_X)
    {
        if (fractional_X >= 1.0) return y2;
        
        return fractional_X * y2 + (1.0 - fractional_X) * y1;
        
    }
    
    AudioBuffer<Type> buffer;
    unsigned int writeIndex;
    unsigned int mBufferLength;
    unsigned int wrapMask;
};


template <typename Type>
class FeedbackMechanism
{

public:
    void createDelayBuffers (double sampleRate, Type bufferLength_mSec)
    {
        mSampleRate = sampleRate;
        mBufferLength_mSec = bufferLength_mSec;

        mSamplesPerMSec = sampleRate / 1000.0;

        mBufferLength = (unsigned int) (mBufferLength_mSec * mSamplesPerMSec) + 1;

        buffer.makeBuffer (mBufferLength);
        
    }


    bool reset (double sampleRate)
    {
        if (mSampleRate == sampleRate)
        {
            buffer.clearBuffer();
            return true;
        }

        createDelayBuffers (sampleRate, mBufferLength_mSec);
        return true;
    }

    void processSample (Type& xn, CircularBuffer<Type>& delayBuffer, Type delayInSamples, unsigned int channel )
    {
        Type yn = delayBuffer.readBuffer (delayInSamples, true, channel);
        Type dn = xn + yn * mFeedback;
        delayBuffer.writeBuffer(dn, channel);
        xn = xn * 0.5 + yn * 0.5;
    }

    void processBuffer (AudioBuffer<Type>& inputBuffer)
    {
        for (unsigned int ch = 0; ch < inputBuffer.getNumChannels(); ++ ch)
        {
            auto* xn = inputBuffer.getWritePointer(ch);
                  for (size_t i = 0; i < inputBuffer.getNumSamples(); ++i)
                  {
                      processSample (xn[i], buffer, mDelayInSamplesL, ch);
                  }
        }

    }
private:
    CircularBuffer<Type> buffer;
    Type mFeedback {0.8};
    Type mBufferLength_mSec;
    Type mWetMix;
    Type mDryMix;
    Type mDelayInSamplesL;
    Type mDelayInSamplesR;
    unsigned int mBufferLength;
    Type mSampleRate;
    Type mSamplesPerMSec;

};
