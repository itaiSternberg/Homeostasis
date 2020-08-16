/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Slot.h"
//==============================================================================
/**
*/
class HomeostasisAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    HomeostasisAudioProcessorEditor (HomeostasisAudioProcessor&);
    ~HomeostasisAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    
    HomeostasisAudioProcessor& processor;
    


    Slot feedbackSlot1;
    Slot feedbackSlot2;
    Slot feedbackSlot3;
    Slot feedbackSlot4;
    
    std::unique_ptr<ComboBoxAttachment> feedbackSlot1Attachment;
    std::unique_ptr<ComboBoxAttachment> feedbackSlot2Attachment;
    std::unique_ptr<ComboBoxAttachment> feedbackSlot3Attachment;
    std::unique_ptr<ComboBoxAttachment> feedbackSlot4Attachment;
    
    Slider delaySlider;
    TextButton panicButton;
    std::unique_ptr<ButtonParameterAttachment> panicButtonAttachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HomeostasisAudioProcessorEditor)
};
