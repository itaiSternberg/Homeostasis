/*
 ==============================================================================
 
 Wavetable.h
 Created: 11 Jul 2020 4:40:27pm
 Author:  Itai Sternberg
 
 ==============================================================================
 */
#pragma once
#include <JuceHeader.h>
#include "ProcessorBase.h"
#include "DelayLine.h"

class SynthSound : public SynthesiserSound
{
public:
    bool appliesToNote(int /*midiNoteNum*/) override
    {
        return true;
    }
    bool appliesToChannel(int /*midiChannel*/) override
    {
        return true;
    }
    
};

class SynthVoice : public SynthesiserVoice
{
public:
    SynthVoice()
    {
    }
    bool canPlaySound (SynthesiserSound* sound) override
    {
        return dynamic_cast <SynthSound*> (sound) != nullptr;
    }
    
    void startNote (int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) override
    {
        
        auto frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        auto sampleRate = getSampleRate();
        delayLineSize = (size_t) juce::roundToInt (sampleRate / frequency);
        burst.resize(delayLineSize);
        randomiseDelayLine(burst);
        
        noteOff = false;
    }
    
    void stopNote (float velocity, bool allowTailOff) override
    {
        clearCurrentNote();
        noteOff = true;

    }
    
    void pitchWheelMoved (int newPitchWheelValue) override
    {
        
    }
    
    void controllerMoved (int controllerNumber, int newControllerValue) override
    {
        
    }
    
    void renderNextBlock (AudioBuffer<float> &outputBuffer, int startSample, int numSamples) override
    {
        if (!noteOff)
        {
            for (int ch = 0; ch < outputBuffer.getNumChannels(); ++ch)
            {
                for (int i = 0; i < outputBuffer.getNumSamples(); ++i)
                {
                    auto burstSample = burst.get(i % delayLineSize);
                    
                    
                    outputBuffer.setSample(ch, i, burstSample);
                }
            }
        }

        
    }
    
    
    int randomSampleGen ()
    {
        if (juce::Random::getSystemRandom().nextBool())
        {
            return 1;
            
        }
        else
        {
            return -1;
        };
    }
    
    void randomiseDelayLine (DelayLine<float>& delayLine)
    {
        
        for (int i = 0; i < delayLine.size(); ++i)
        {
            float sample = juce::Random::getSystemRandom().nextFloat();
            sample = (sample * 2) - 1;
            delayLine.push(sample);
        }
    }
private:
    float damping;
    size_t delayLineSize;
    DelayLine <float> burst;
//    DelayLine <float> delayLine;
    bool noteOff {true};
    
};

class Wavetable : public ProcessorBase
{
public:
    Wavetable();
    ~Wavetable();
    const juce::String getName() const override { return "Source"; }
    bool acceptsMidi() const override {return true;}
    
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer& midiMessages) override;
    void reset() override;
    void resizeDelayLineToFreq (DelayLine<float> &delayLine, int lastSampleRate, const juce::MidiMessageMetadata &metadata);

   
private:
    juce::Synthesiser synth;
    DelayLine<float> delayLine;
    size_t delayLineSize;
    int lastSampleRate;
};


