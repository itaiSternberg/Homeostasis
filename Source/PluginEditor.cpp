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
, masterSlot1 (p.mainTree, "5")
, masterSlot2 (p.mainTree, "6")
, masterSlot3 (p.mainTree, "7")
, masterSlot4 (p.mainTree, "8")
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
    
    
    addAndMakeVisible (&masterSlot1);
    masterSlot1Attachment = std::make_unique<ComboBoxAttachment> (processor.mainTree,"slot1",feedbackSlot1.fxMenu);
    
    addAndMakeVisible (&masterSlot2);
    masterSlot2Attachment = std::make_unique<ComboBoxAttachment> (processor.mainTree,"slot2",feedbackSlot2.fxMenu);
    
    addAndMakeVisible (&masterSlot3);
    masterSlot3Attachment = std::make_unique<ComboBoxAttachment> (processor.mainTree,"slot3",feedbackSlot3.fxMenu);
    
    addAndMakeVisible (&masterSlot4);
    masterSlot4Attachment = std::make_unique<ComboBoxAttachment> (processor.mainTree,"slot4",feedbackSlot4.fxMenu);
    
    
    
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
    auto constexpr slotYMargin = 30;
    auto constexpr slotXmargin = 40;
    auto constexpr panicButtonLength = 40;
    
    feedbackSlot1.setBounds (slotXmargin , slotYMargin, slotWidth, slotHeight);
    feedbackSlot2.setBounds (2 * slotXmargin + slotWidth, slotYMargin, slotWidth, slotHeight);
    feedbackSlot3.setBounds (3 * slotXmargin + 2 * slotWidth, slotYMargin, slotWidth, slotHeight);
    feedbackSlot4.setBounds (4 * slotXmargin + 3 * slotWidth, slotYMargin, slotWidth, slotHeight);
    
    panicButton.setBounds((getWidth() - panicButtonLength) / 2, (getHeight() - panicButtonLength) / 2, panicButtonLength, panicButtonLength);
    
    
    masterSlot1.setBounds (slotXmargin , getHeight () - slotYMargin - slotHeight, slotWidth, slotHeight);
    masterSlot2.setBounds (2 * slotXmargin + slotWidth, getHeight () - slotYMargin - slotHeight, slotWidth, slotHeight);
    masterSlot3.setBounds (3 * slotXmargin + 2 * slotWidth, getHeight () - slotYMargin - slotHeight, slotWidth, slotHeight);
    masterSlot4.setBounds (4 * slotXmargin + 3 * slotWidth, getHeight () - slotYMargin - slotHeight, slotWidth, slotHeight);

    
}


