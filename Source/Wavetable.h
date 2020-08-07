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
        damping = 0.99;
    }
    bool canPlaySound (SynthesiserSound* sound) override
    {
        return dynamic_cast <SynthSound*> (sound) != nullptr;
    }
    
    void startNote (int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) override
    {
        
        auto frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        auto sampleRate = getSampleRate();
        
        auto delayLineSize = (size_t) juce::roundToInt (sampleRate / frequency);
        delayLine.resize(delayLineSize);
        
        randomiseDelayLine(delayLine);
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
                    auto delayLineSample = delayLine.get(i % delayLine.size());
                    outputBuffer.addSample(ch, i, delayLineSample);
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
            auto sample = randomSampleGen();
            delayLine.push(sample);
        }
    }
private:
    float damping;
    DelayLine <float> delayLine;
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
    
private:
    juce::MidiKeyboardState keyboardState;
    juce::Synthesiser synth;
};


