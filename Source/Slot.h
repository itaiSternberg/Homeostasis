/*
  ==============================================================================

    Slot.h
    Created: 12 Jun 2020 6:14:00pm
    Author:  Itai Sternberg

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SVF.h"
#include "Distortion.h"
#include "Phaser.h"
//==============================================================================
/*
*/
class Slot    : public Component,
                public ComboBox::Listener
{
public:
    Slot(AudioProcessorValueTreeState& apvts, String slotIndex);
    ~Slot();

    void paint (Graphics&) override;
    void resized() override;
    void displaySelectedModule (ComboBox* ComboBox);
    void comboBoxChanged (ComboBox* ComboBox) override;
    ComboBox fxMenu;
private:
    
    AudioProcessorValueTreeState& apvts;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> fxMenuAttach;
    StringArray fxMenuOptions;
    String slotIndex;
    std::unique_ptr<Component> selectedFx  {nullptr};
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Slot)
};


