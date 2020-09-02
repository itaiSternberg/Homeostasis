/*
  ==============================================================================

    Distortion.cpp
    Created: 11 Jul 2020 1:08:30am
    Author:  Itai Sternberg

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Distortion.h"

//==============================================================================
Distortion::Distortion(AudioProcessorValueTreeState& apvts, String slotIndex)
: apvts(apvts)
, driveSlider(Slider::SliderStyle::RotaryVerticalDrag,Slider::NoTextBox)
, outputSlider(Slider::SliderStyle::RotaryVerticalDrag,Slider::NoTextBox)
, driveLabel("driveLabel","Drive")
, outputLabel("outputLabel", "Output")
{
    addAndMakeVisible(&driveSlider);
    driveSliderAttachment = std::make_unique<SliderAttachment>(apvts, "drive" + slotIndex, driveSlider);
    
    addAndMakeVisible(&outputSlider);
    outputSliderAttachment = std::make_unique<SliderAttachment>(apvts, "distortionGain" + slotIndex, outputSlider);
    
    addAndMakeVisible(&driveLabel);
    addAndMakeVisible(&outputLabel);
}

Distortion::~Distortion()
{
}

void Distortion::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
           Rectangle<float> frame (10, 10, getWidth() - 20 , getHeight() - 20);
           g.setColour (Colours::white);
           g.drawRoundedRectangle(frame, 3.5f, 1.5f);
    driveLabel.setJustificationType(Justification::centred);
    outputLabel.setJustificationType(Justification::centred);
}

void Distortion::resized()
{
    driveSlider.setBounds(20, 100, 70, 70);
    outputSlider.setBounds(110, 100, 70, 70);
    
    driveLabel.setBounds(20, 160 , 45 , 30 );
    outputLabel.setBounds(110, 160, 45 , 30 );
  
}
