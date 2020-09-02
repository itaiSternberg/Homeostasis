/*
  ==============================================================================

    SVF.h
    Created: 11 Jun 2020 12:55:15pm
    Author:  Itai Sternberg

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;

//==============================================================================
/*
*/
class SVF    : public Component
{
public:
    SVF(AudioProcessorValueTreeState& apvts, String slotIndex);
    ~SVF();

    void paint (Graphics&) override;
    void resized() override;

private:
    
    Slider mCutoffFreqSlider;
    std::unique_ptr<SliderAttachment> mCutoffAttach;
    
    Slider mResonanceSlider;
    std::unique_ptr<SliderAttachment> mResAttach;
    
    ComboBox mFilterTypeBox;
    std::unique_ptr<ComboBoxAttachment> mFilterTypeAttach;
    
    Label cutOffFreqLabel;
    Label resonanceLabel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SVF)
};
