/*
  ==============================================================================

    Phaser.cpp
    Created: 26 Jul 2020 4:25:34pm
    Author:  Itai Sternberg

  ==============================================================================
*/

#include "Phaser.h"

Phaser::Phaser (AudioProcessorValueTreeState& apvts, String slotIndex)
: apvts(apvts)
, freqSlider(Slider::SliderStyle::RotaryVerticalDrag,Slider::NoTextBox)
, rateSlider(Slider::SliderStyle::RotaryVerticalDrag,Slider::NoTextBox)
, depthSlider(Slider::SliderStyle::RotaryVerticalDrag,Slider::NoTextBox)
, feedbackSlider(Slider::SliderStyle::RotaryVerticalDrag,Slider::NoTextBox)
, mixSlider(Slider::SliderStyle::RotaryVerticalDrag,Slider::NoTextBox)
, freqLabel("freq", "C. Freq")
, rateLabel("rate", "Rate")
, depthLabel("depth","Depth")
, feedbackLabel("feedback","Feedback")
, mixLabel("mix","Mix")
{
    addAndMakeVisible(&rateSlider);
    rateAttach = std::make_unique<SliderAttachment>(apvts, "phaserRate" + slotIndex, rateSlider);

    addAndMakeVisible(&depthSlider);
    depthAttach = std::make_unique<SliderAttachment>(apvts, "phaserDepth" + slotIndex, depthSlider);
    
    addAndMakeVisible(&feedbackSlider);
    feedbackAttach = std::make_unique<SliderAttachment>(apvts, "phaserFeedback" + slotIndex, feedbackSlider);
    
    addAndMakeVisible(&mixSlider);
    mixAttach = std::make_unique<SliderAttachment>(apvts, "phaserMix" + slotIndex, mixSlider);
    
    addAndMakeVisible(&freqLabel);
    addAndMakeVisible(&rateLabel);
    addAndMakeVisible(&depthLabel);
    addAndMakeVisible(&mixLabel);
    addAndMakeVisible(&feedbackLabel);
}


Phaser::~Phaser ()
{
    
}

void Phaser::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
           Rectangle<float> frame (10, 10, getWidth() - 20 , getHeight() - 20);
           g.setColour (Colours::white);
           g.drawRoundedRectangle(frame, 3.5f, 1.5f);
}

void Phaser::resized()
{
    rateSlider.setBounds(20, 20, 60, 60);
    rateLabel.setBounds(20, 70, 45 , 30 );
    rateLabel.setJustificationType(Justification::centred);

    depthSlider.setBounds(110, 20, 60, 60);
    depthLabel.setBounds(110, 70, 45 , 30 );
    depthLabel.setJustificationType(Justification::centred);

    mixSlider.setBounds(70, 70, 60, 60);
    mixLabel.setJustificationType(Justification::centred);
    mixLabel.setBounds(70, 120, 45 , 30 );

    feedbackSlider.setBounds(20, 120, 60, 60);
    feedbackLabel.setJustificationType(Justification::centred);
    feedbackLabel.setBounds(20, 160 , 50 , 30 );

    freqSlider.setBounds(110, 120, 60, 60);
    freqLabel.setJustificationType(Justification::centred);
    freqLabel.setBounds(110, 160, 45 , 30 );

   
    

    

 
}
