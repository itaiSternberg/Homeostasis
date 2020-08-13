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


template <typename Type>
class CircularBuffer
{
public:
    void makeBuffer (unsigned int bufferLength)
    {
        mBufferLength = (unsigned int) (pow (2, ceil (log (bufferLength) / log(2))));
        wrapMask = mBufferLength - 1;
        buffer.resize (bufferLength);
        clearBuffer ();
    }
    void clearBuffer ()
    {
        std::fill (buffer.begin(), buffer.end() , Type (0));
    }
    
    void writeBuffer (Type newValue)
    {
        buffer[writeIndex++] = newValue;  // writes the newsample to the position of the writeindex then increments it
        
        writeIndex &= wrapMask;          // wraps around the size of the buffer
    }
    
    Type readBuffer (int delayInSamples)
    {
        
        int readIndex = writeIndex - delayInSamples;
        readIndex &= wrapMask;            // wraps the readIndex
        return buffer [readIndex];         // returns the sample at the buffer
    }
    
    Type readBuffer (double delayInFractionalSamples, bool interpolate)
    {
        Type y1 =  readBuffer ((int) delayInFractionalSamples);
        if (!interpolate) return y1;
        
        Type y2 = (int) readBuffer ((int) delayInFractionalSamples + 1);
        
        double frac = delayInFractionalSamples - (int) delayInFractionalSamples;
        
        return linearInterpolation (y1, y2, frac);
    }
    
    
private:
   static double linearInterpolation (double y1, double y2, double fractional_X)
    {
        if (fractional_X >= 1.0) return y2;
        
        return fractional_X * y2 + (1.0 - fractional_X) * y1;
        
    }
    std::vector<Type> buffer;
    unsigned int writeIndex;
    unsigned int mBufferLength;
    unsigned int wrapMask;
};


class FeedbackMechanisim
{
    
public:
    void createDelayBuffers (double sampleRate, double bufferLength_mSec)
    {
        mSampleRate = sampleRate;
        mBufferLength_mSec = bufferLength_mSec;
        
        mSamplesPerMSec = sampleRate / 1000.0;
        
        mBufferLength = (unsigned int) (mBufferLength_mSec * mSamplesPerMSec) + 1;
        for (auto buffer : buffers)
        {
            buffer.makeBuffer (mBufferLength);
        }
    }
    
    
    bool reset (double sampleRate)
    {
        if (mSampleRate == sampleRate)
        {
            for (auto buffer : buffers)
                   {
                       buffer.clearBuffer();
                   }
            return true;
        }
        
        createDelayBuffers (sampleRate, mBufferLength_mSec);
        return true;
    }
    
    void processSample (double& xn, CircularBuffer<double>& delayBuffer, double delayInSamples )
    {
        double yn = delayBuffer.readBuffer (delayInSamples, true);
        double dn = xn + yn * mFeedback;
        delayBuffer.writeBuffer(dn);
        xn = xn * 0.5 + yn * 0.5;
    }
    
    void processBuffer (AudioBuffer<double>& inputBuffer)
    {
        for (unsigned int ch = 0; ch < inputBuffer.getNumSamples(); ++ ch)
        {
            auto delayBuffer = buffers[ch];
            auto* xn = inputBuffer.getWritePointer(ch);
                  for (unsigned int i = 0; i < inputBuffer.getNumSamples(); ++i)
                  {
                      processSample (xn[i], delayBuffer, mDelayInSamplesL);
                  }
        }
      
    }
private:
    std::array<CircularBuffer<double>, 2> buffers;
    double mFeedback {80};
    double mBufferLength_mSec;
    double mWetMix;
    double mDryMix;
    double mDelayInSamplesL;
    double mDelayInSamplesR;
    unsigned int mBufferLength;
    double mSampleRate;
    double mSamplesPerMSec;
    
};
