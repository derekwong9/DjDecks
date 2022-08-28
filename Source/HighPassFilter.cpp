/*
  ==============================================================================

    HighPassFilter.cpp
    Created: 28 Aug 2022 2:52:46pm
    Author:  derek

  ==============================================================================
*/

#include <JuceHeader.h>
#include "HighPassFilter.h"

//==============================================================================
HighPassFilter::HighPassFilter()
{
  // In your constructor, you should add any child components, and
  // initialise any special settings that your component needs.
  frequency = 0;
  resanance = 0;
}

HighPassFilter::~HighPassFilter()
{
}
void HighPassFilter::setFrequency(const float inputFreq)
{
  frequency = inputFreq;
}
void HighPassFilter::setResanance(const float inputRes)
{
  resanance = inputRes;
}
void HighPassFilter::prepareToPlay(double sampleRate, int samplesPerBlock)
{
  lastSampleRate = sampleRate;

  dsp::ProcessSpec spec;
  spec.sampleRate = sampleRate;
  spec.maximumBlockSize = samplesPerBlock;
  spec.numChannels = 2;

  highPassFilter.prepare(spec);
  highPassFilter.reset();
}
void HighPassFilter::updateFilter()
{
  *highPassFilter.state = *dsp::IIR::Coefficients<float>::makeHighPass(lastSampleRate, frequency, resanance);
}
void HighPassFilter::processBlock(AudioBuffer<float> &buffer)
{
  for (int i = 0; i < 2; ++i)
  {
    buffer.clear(i, 0, buffer.getNumSamples());
  }

  dsp::AudioBlock<float> block(buffer);
  updateFilter();
  highPassFilter.process(dsp::ProcessContextReplacing<float>(block));
}