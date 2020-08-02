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
        makeTable();
    }
    bool canPlaySound (SynthesiserSound* sound) override
    {
        return dynamic_cast <SynthSound*> (sound) != nullptr;
    }
    
    void startNote (int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) override
    {
        auto frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        auto tableSizeOverSampleRate = tableSize / getSampleRate();
        tableDelta = frequency * tableSizeOverSampleRate;
        DBG(String(midiNoteNumber));
        currentIndex = 0.0f;
    }
    
    void stopNote (float velocity, bool allowTailOff) override
    {
        clearCurrentNote();
    }
    
    void pitchWheelMoved (int newPitchWheelValue) override
    {
        
    }
    
    void controllerMoved (int controllerNumber, int newControllerValue) override
    {
        
    }
    
    void renderNextBlock (AudioBuffer<float> &outputBuffer, int startSample, int numSamples) override
    {
        auto tableSize = (unsigned int)  randomTable.getNumSamples();

        auto index0 = (unsigned int) currentIndex;
        auto index1 = index0 + 1;

        auto frac = currentIndex - (float) index0;

        for (auto i = 0; i < outputBuffer.getNumChannels(); ++i)
        {
            auto* table = randomTable.getReadPointer(i);

            auto value0 = table[index0];
            auto value1 = table[index1];
            auto currentSample = value0 + frac * (value1 - value0);
            if ((currentIndex += tableDelta) > (float) tableSize)
                currentIndex -= (float) tableSize;

            outputBuffer.addSample (i, startSample, currentSample);

        }
        




        
    }
    
    void makeTable ()
    {
        randomTable.setSize(2, tableSize + 1);
        auto* samplesLeft = randomTable.getWritePointer(0);
        auto* samplesRight = randomTable.getWritePointer(1);
        
        for (unsigned int i = 0; i < tableSize; ++i)
        {
            int sample = randomSampleGen();
            samplesLeft[i] = sample;
            samplesRight[i] = sample;
        }
        
                for (auto j = 0; j < tableSize; ++j)
                {
                    auto readLeft = randomTable.getReadPointer(0);
                    auto readRight = randomTable.getReadPointer(0);
                    DBG(String(readLeft[j]) + " " + String (readRight[j]));
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
private:
    unsigned int tableSize = 1 << 7;
    AudioSampleBuffer randomTable;
    float currentIndex = 0.0f, tableDelta = 0.0f;
};

//============================================================
class SynthAudioSource : public juce::AudioSource
{
public:
    SynthAudioSource (juce::MidiKeyboardState& keyState)
    : keyboardState (keyState)
    {
        synth.addVoice(new SynthVoice());
        synth.addSound(new SynthSound());
    }
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        synth.setCurrentPlaybackSampleRate(sampleRate);
    }
    
    void releaseResources () override
    {
    }
    
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        bufferToFill.clearActiveBufferRegion();
        
        juce::MidiBuffer incomingMidi;
//        keyboardState.processNextMidiBuffer(incomingMidi, bufferToFill.startSample, bufferToFill.numSamples, true);
        
        synth.renderNextBlock(*bufferToFill.buffer, incomingMidi, bufferToFill.startSample, bufferToFill.numSamples);
    }
    
    
private:
    
    juce::MidiKeyboardState& keyboardState;
    juce::Synthesiser synth;
    float currentIndex = 0.0f, tableDelta = 0.0f;

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
    SynthAudioSource synthAudioSource;

};


