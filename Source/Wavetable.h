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
class Wavetable : public ProcessorBase
{
public:
    Wavetable();
    ~Wavetable();
    const juce::String getName() const override { return "Source"; }
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void processBlock (juce::AudioSampleBuffer& buffer, juce::MidiBuffer&) override;
    void reset() override;

private:
    
};

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
  bool canPlaySound (SynthesiserSound* sound) override
    {
        return dynamic_cast <SynthSound*> (sound) != nullptr;
    }
    
   void startNote (int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition) override
    {
        randomTable.setFrequency(midiNoteNumber);
        randomTable.initialise([this] (float x) {if (random.nextInt() % 2 == 0) {return -x;} { return x;} });
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
        
      

    }
private:
    Random random;
    dsp::Oscillator<float> randomTable;

};

//============================================================
class SynthAudioSource : public juce::AudioSource
{
public:
    SynthAudioSource (juce::MidiKeyboardState& keyState)
    : keyboardState (keyState)
    {
        
    }
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
    }
    
    void releaseResources () override
    {
        
    }
    
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        
    }
private:
    juce::MidiKeyboardState& keyboardState;
    Wavetable wavetable;
};
