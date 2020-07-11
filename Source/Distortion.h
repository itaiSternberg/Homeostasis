/*
  ==============================================================================

    Distortion.h
    Created: 11 Jul 2020 1:08:30am
    Author:  Itai Sternberg

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

//==============================================================================
/*
*/
class Distortion  : public juce::Component
{
public:
    Distortion(AudioProcessorValueTreeState& apvts, String slotIndex);
    ~Distortion() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    AudioProcessorValueTreeState& apvts;
    Slider driveSlider;
    Slider outputSlider;
    std::unique_ptr <SliderAttachment> driveSliderAttachment;
    std::unique_ptr <SliderAttachment> outputSliderAttachment;
    Label driveLabel;
    Label outputLabel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Distortion)
};
