/*
  ==============================================================================

    Phaser.h
    Created: 26 Jul 2020 4:25:34pm
    Author:  Itai Sternberg

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
//==============================================================================
/*
*/

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

class Phaser    : public Component
{
public:
    Phaser(AudioProcessorValueTreeState& apvts, String slotIndex);
    ~Phaser();

    void paint (Graphics&) override;
    void resized() override;

private:
    AudioProcessorValueTreeState& apvts;
        
    Slider rateSlider;
    std::unique_ptr<SliderAttachment> rateAttach;
    
    Slider depthSlider;
    std::unique_ptr<SliderAttachment> depthAttach;

    Slider feedbackSlider;
    std::unique_ptr<SliderAttachment> feedbackAttach;

    Slider mixSlider;
    std::unique_ptr<SliderAttachment> mixAttach;

    Label freqLabel;
    Label rateLabel;
    Label depthLabel;
    Label feedbackLabel;
    Label mixLabel;
};
