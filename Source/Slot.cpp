/*
  ==============================================================================

    Slot.cpp
    Created: 12 Jun 2020 6:14:00pm
    Author:  Itai Sternberg

  ==============================================================================
*/

#include "Slot.h"

//==============================================================================
Slot::Slot (AudioProcessorValueTreeState& apvts, String slotIndex)
: fxMenu()
, apvts(apvts)
, fxMenuOptions({"Empty","Filter","Phaser","Distortion"})
, slotIndex(slotIndex)
, selectedFx (std::make_unique<Label> ("Empty"))
{
    addAndMakeVisible(&fxMenu);
    fxMenu.addListener(this);
    fxMenu.addItemList(fxMenuOptions, 1);
    
    
    setSize(200,230);
}

Slot::~Slot()
{
}

void Slot::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 3.5, 2);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    
}

void Slot::resized()
{
    fxMenu.setBounds(0, 0, getWidth(), 25);

}

void Slot::comboBoxChanged(ComboBox* ComboBox)
{
    bool is_label = true;
    Component::removeChildComponent(selectedFx.get());
    selectedFx = nullptr;
    
    if (ComboBox->getText() == "Empty")
    {
        selectedFx = std::make_unique<Label> ("","Empty");
    } else if (ComboBox->getText() == "Filter")
    {
        selectedFx = std::make_unique<SVF> (apvts, slotIndex);
        is_label = false;
    } else if (ComboBox->getText() == "Phaser")
    {
        selectedFx = std::make_unique<Label> ("","Phaser");
    } else if (ComboBox->getText() == "Distortion")
    {
        selectedFx = std::make_unique<Label> ("","Distortion");
    }
    
    Component::addAndMakeVisible(selectedFx.get());
    selectedFx->setBounds(5, 26, getWidth() - 10, getHeight() - 30);
    
   
    if (is_label)
    {
        dynamic_cast<Label*>(selectedFx.get())->setJustificationType(Justification::centred);
    }
    
    repaint();


}


