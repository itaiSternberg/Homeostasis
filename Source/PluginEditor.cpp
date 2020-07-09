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
, feedbackSlot1(p.mainTree, "1")
, feedbackSlot2(p.mainTree, "2")
, feedbackSlot3(p.mainTree, "3")
, feedbackSlot4(p.mainTree, "4")
{
    // Displaying Feedback Slots and attaching to parameters
    addAndMakeVisible(feedbackSlot1);
    feedbackSlot1Attachment = std::make_unique<ComboBoxAttachment> (processor.mainTree,"slot1",feedbackSlot1.fxMenu);
    
    addAndMakeVisible(feedbackSlot2);
    feedbackSlot2Attachment = std::make_unique<ComboBoxAttachment> (processor.mainTree,"slot2",feedbackSlot2.fxMenu);

    addAndMakeVisible(feedbackSlot3);
    feedbackSlot3Attachment = std::make_unique<ComboBoxAttachment> (processor.mainTree,"slot3",feedbackSlot3.fxMenu);

    addAndMakeVisible(feedbackSlot4);
    feedbackSlot4Attachment = std::make_unique<ComboBoxAttachment> (processor.mainTree,"slot4",feedbackSlot4.fxMenu);
   
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
    
    feedbackSlot1.setBounds(40 , 50, feedbackSlot1.getWidth(),feedbackSlot1.getHeight());
    feedbackSlot2.setBounds(280, 50, feedbackSlot2.getWidth(),feedbackSlot2.getHeight());
    feedbackSlot3.setBounds(520, 50, feedbackSlot3.getWidth(),feedbackSlot3.getHeight());
    feedbackSlot4.setBounds(760, 50, feedbackSlot4.getWidth(),feedbackSlot4.getHeight());


    
}


