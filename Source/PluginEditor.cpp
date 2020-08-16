/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HomeostasisAudioProcessorEditor::HomeostasisAudioProcessorEditor (HomeostasisAudioProcessor& p)
: AudioProcessorEditor (&p)
, processor (p)
, feedbackSlot1 (p.mainTree, "1")
, feedbackSlot2 (p.mainTree, "2")
, feedbackSlot3 (p.mainTree, "3")
, feedbackSlot4 (p.mainTree, "4")
, delaySlider (Slider::SliderStyle::LinearHorizontal, Slider::TextBoxBelow)
, panicButton ("!")
{
    // Displaying Feedback Slots and attaching to parameters
    addAndMakeVisible (&feedbackSlot1);
    feedbackSlot1Attachment = std::make_unique<ComboBoxAttachment> (processor.mainTree,"slot1",feedbackSlot1.fxMenu);
    
    addAndMakeVisible (&feedbackSlot2);
    feedbackSlot2Attachment = std::make_unique<ComboBoxAttachment> (processor.mainTree,"slot2",feedbackSlot2.fxMenu);

    addAndMakeVisible (&feedbackSlot3);
    feedbackSlot3Attachment = std::make_unique<ComboBoxAttachment> (processor.mainTree,"slot3",feedbackSlot3.fxMenu);

    addAndMakeVisible (&feedbackSlot4);
    feedbackSlot4Attachment = std::make_unique<ComboBoxAttachment> (processor.mainTree,"slot4",feedbackSlot4.fxMenu);
   
    addAndMakeVisible (&delaySlider);
    
    addAndMakeVisible(&panicButton);
    panicButtonAttachment = std::make_unique<ButtonParameterAttachment> (*processor.mainTree.getParameter("panic"), panicButton);
    setSize (1000, 600);
}

HomeostasisAudioProcessorEditor::~HomeostasisAudioProcessorEditor()
{
}

//==============================================================================
void HomeostasisAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void HomeostasisAudioProcessorEditor::resized()
{

    //Position feedback slots
    auto slotHeight = feedbackSlot1.getHeight();
    auto slotWidth = feedbackSlot1.getWidth();
    
    feedbackSlot1.setBounds (40 , 50, slotWidth, slotHeight);
    feedbackSlot2.setBounds (280, 50, slotWidth, slotHeight);
    feedbackSlot3.setBounds (520, 50, slotWidth, slotHeight);
    feedbackSlot4.setBounds (760, 50, slotWidth, slotHeight);
    
    delaySlider.setBounds ((getWidth() - 600) / 2, 70 + slotHeight, 600, 30);
    panicButton.setBounds((getWidth() - 50) / 2, 130  + slotHeight, 50, 50);

    
}


