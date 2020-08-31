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
class CircularBuffer
{
public:
    void makeBuffer (unsigned int bufferLength)
    {
        writeIndex = 0;
        mBufferLength = (unsigned int) (pow (2, ceil (log (bufferLength) / log(2))));
        wrapMask = mBufferLength - 1;
        buffer.setSize (1, mBufferLength);
        clearBuffer ();
        writePointer = buffer.getWritePointer (0);
        readPointer = buffer.getReadPointer (0);
    }
    
    void clearBuffer ()
    {
        buffer.clear();
    }
    
    void writeBuffer (Type newValue)
    {
        writePointer[writeIndex++] = newValue;  // writes the newsample to the position of the writeindex then increments it
        writeIndex &= wrapMask; // wraps around the size of the buffer
    }
    
    AudioBuffer<Type> readBufferAsBuffer ( unsigned int delayInSamples )
    {
        int readIndex = writeIndex - delayInSamples;
        readIndex &= wrapMask;
        
        auto* writePointer = buffer.getWritePointer (0);
        AudioBuffer<Type> tempBuffer (&writePointer, 1, readIndex, 1);
        return tempBuffer;
    }
    
    Type readBuffer (int delayInSamples)
    {
                 int readIndex = writeIndex - delayInSamples;
        readIndex &= wrapMask;            // wraps the readIndex
        return readPointer [readIndex];         // returns the sample at the buffer
    }
    
    Type readBuffer (double delayInFractionalSamples, bool interpolate)
    {
        Type y1 =  readBuffer ((int) delayInFractionalSamples);
        if (!interpolate) return y1;
        
        Type y2 = (int) readBuffer ((int) (delayInFractionalSamples + 1));
        
        double frac = delayInFractionalSamples - (int) delayInFractionalSamples;
        
        return linearInterpolation (y1, y2, frac);
    }
    
    void scrambleBuffer ()
    {
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            float randomSample = randomSampleGen();
            writeBuffer(randomSample);
        }
    }
    float* writePointer;
    const float* readPointer;
    AudioBuffer<Type> buffer;
    unsigned int writeIndex;
    unsigned int mBufferLength;
    unsigned int wrapMask;
private:
    float randomSampleGen ()
    {
        if (juce::Random::getSystemRandom().nextBool())
        {
            return 0.2;
        }
        else
        {
            return -0.2;
        }
    }
};

