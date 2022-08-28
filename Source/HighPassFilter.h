/*
  ==============================================================================

    HighPassFilter.h
    Created: 28 Aug 2022 2:52:46pm
    Author:  derek

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
An IRR High Pass Filter
 */
class HighPassFilter
{
public:
  HighPassFilter();
  ~HighPassFilter();

  void prepareToPlay(double sampleRate, int samplesPerBlock);
  void updateFilter();
  void processBlock(AudioBuffer<float> &);

  void setFrequency(const float);
  void setResanance(const float);

private:
  dsp::ProcessorDuplicator<dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>> highPassFilter;
  float lastSampleRate;

  float frequency;
  float resanance;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HighPassFilter)
};
