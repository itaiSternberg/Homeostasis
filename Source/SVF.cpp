/*
  ==============================================================================

    SVF.cpp
    Created: 11 Jun 2020 12:55:15pm
    Author:  Itai Sternberg

  ==============================================================================
*/

#include "SVF.h"

//==============================================================================
SVF::SVF(AudioProcessorValueTreeState& apvts, String slotIndex)
: mCutoffFreqSlider(Slider::SliderStyle::RotaryVerticalDrag,Slider::NoTextBox)
, mResonanceSlider(Slider::SliderStyle::RotaryVerticalDrag,Slider::NoTextBox)
, mFilterTypeBox("Filter Type")
, cutOffFreqLabel("cuttoffFreqLabel", "Cutoff")
, resonanceLabel("resonanceLabel", "Res.")

{
    Array<String> filterTypes {"HP","LP","BP"};
    mFilterTypeBox.addItemList(filterTypes, 1);
    
    addAndMakeVisible(&mCutoffFreqSlider);
    mCutoffAttach = std::make_unique<SliderAttachment>(apvts, "cutoff" + slotIndex, mCutoffFreqSlider);
    
    addAndMakeVisible(&mResonanceSlider);
    mResAttach = std::make_unique<SliderAttachment>(apvts, "res" + slotIndex, mResonanceSlider);
    
    addAndMakeVisible(&mFilterTypeBox);
    mFilterTypeAttach = std::make_unique<ComboBoxAttachment> (apvts, "filterType" + slotIndex, mFilterTypeBox);
    
    addAndMakeVisible(&cutOffFreqLabel);
    addAndMakeVisible(&resonanceLabel);
}

SVF::~SVF()
{
}

void SVF::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    Rectangle<float> frame (10, 10, getWidth() - 20 , getHeight() - 20);
    g.setColour (Colours::white);
    g.drawRoundedRectangle(frame, 3.5f, 1.5f);
}

void SVF::resized()
{
    mCutoffFreqSlider.setBounds(20, 100, 70, 70);
    mResonanceSlider.setBounds(110, 100, 70, 70);
    mFilterTypeBox.setBounds(20, 20, getWidth() - 40, 45);
    
    cutOffFreqLabel.setBounds(20, 160 , 45 , 30 );
    resonanceLabel.setBounds(110, 160, 45 , 30 );
    resonanceLabel.setJustificationType(Justification::centred);
    cutOffFreqLabel.setJustificationType(Justification::centred);
}
