/*
  ==============================================================================

    SliderLookAndFeel.h
    Created: 30 Aug 2022 8:45:34pm
    Author:  derek

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
 */
class DeckGuiLookAndFeel : public juce::LookAndFeel_V4
{
public:
  DeckGuiLookAndFeel();
  ~DeckGuiLookAndFeel() override;
  void drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos,
                        const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider &) override;
  void drawButtonBackground(juce::Graphics &g, juce::Button &button, const juce::Colour &backgroundColour,
                            bool, bool isButtonDown) override;

  void drawLinearSliderBackground(Graphics &g,
                                  int x,
                                  int y,
                                  int width,
                                  int height,
                                  float sliderPos,
                                  float minSliderPos,
                                  float maxSliderPos,
                                  const Slider::SliderStyle,
                                  Slider &) override;

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGuiLookAndFeel)
};
