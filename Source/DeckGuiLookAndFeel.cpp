/*
  ==============================================================================

    SliderLookAndFeel.cpp
    Created: 30 Aug 2022 8:45:34pm
    Author:  derek

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGuiLookAndFeel.h"

//==============================================================================
DeckGuiLookAndFeel::DeckGuiLookAndFeel()
{
  // In your constructor, you should add any child components, and
  // initialise any special settings that your component needs.
}

DeckGuiLookAndFeel::~DeckGuiLookAndFeel()
{
}

void DeckGuiLookAndFeel::drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos,
                                          const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider &)
{
  auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
  auto centreX = (float)x + (float)width * 0.5f;
  auto centreY = (float)y + (float)height * 0.5f;
  auto rx = centreX - radius;
  auto ry = centreY - radius;
  auto rw = radius * 2.0f;
  auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

  // fill
  g.setColour(juce::Colours::blueviolet);
  g.fillEllipse(rx, ry, rw, rw);

  // outline
  g.setColour(juce::Colours::navy);
  g.drawEllipse(rx, ry, rw, rw, 3.0f);

  juce::Path p;
  auto pointerLength = radius * 0.33f;
  auto pointerThickness = 5.0f;
  p.addRectangle(-pointerThickness * 1.0f, -radius, pointerThickness, pointerLength);
  p.applyTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));

  // pointer
  g.setColour(juce::Colours::violet);
  g.fillPath(p);
}

void DeckGuiLookAndFeel::drawButtonBackground(juce::Graphics &g, juce::Button &button, const juce::Colour &backgroundColour,
                                              bool, bool isButtonDown)
{
  if (button.getState() == Button::ButtonState::buttonDown)
  {
    button.setColour(TextButton::buttonColourId, Colours::cyan);
  }
  else
  {
    button.setColour(TextButton::buttonColourId, Colours::darkcyan);
  }
  auto buttonArea = button.getLocalBounds();
  auto edge = 4;

  buttonArea.removeFromLeft(edge);
  buttonArea.removeFromTop(edge);

  // shadow
  g.setColour(juce::Colours::darkgrey.withAlpha(0.5f));
  g.fillRect(buttonArea);

  auto offset = isButtonDown ? -edge / 2 : -edge;
  buttonArea.translate(offset, offset);

  g.setColour(backgroundColour);
  g.fillRect(buttonArea);
}