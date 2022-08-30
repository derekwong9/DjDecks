/*
  ==============================================================================

    DeckGUI.h
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <array>
#include <vector>
#include <string>

#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "PlaylistComponent.h"
#include "DeckGuiLookAndFeel.h"
//==============================================================================
/*
 */
class DeckGUI : public Component,
                public Button::Listener,
                public Slider::Listener,
                public FileDragAndDropTarget,
                public Timer
{
public:
  DeckGUI(DJAudioPlayer *player,
          AudioFormatManager &formatManagerToUse,
          AudioThumbnailCache &cacheToUse,
          PlaylistComponent *playlistComponent);
  ~DeckGUI();

  void paint(Graphics &) override;
  void resized() override;

  /** implement Button::Listener */
  void buttonClicked(Button *) override;

  /** implement Slider::Listener */
  void sliderValueChanged(Slider *slider) override;

  bool isInterestedInFileDrag(const StringArray &files) override;
  void filesDropped(const StringArray &files, int x, int y) override;

  void timerCallback() override;

private:
  // active buttons
  TextButton playButton{"PLAY"};
  TextButton stopButton{"STOP"};
  TextButton loadButton{"LOAD"};
  TextButton addToListButton{"ADD TO LIST"};
  // sliders
  Slider volSlider{Slider::Rotary, Slider::TextEntryBoxPosition::TextBoxBelow};
  Slider speedSlider{Slider::Rotary, Slider::TextEntryBoxPosition::TextBoxBelow};
  Slider posSlider;
  Slider filterFreqSlider{Slider::Rotary, Slider::TextEntryBoxPosition::TextBoxBelow};
  Slider filterResSlider{Slider::Rotary, Slider::TextEntryBoxPosition::TextBoxBelow};

  // components
  FileChooser fChooser{"Select a file..."};
  WaveformDisplay waveformDisplay;
  DJAudioPlayer *player;
  PlaylistComponent *playlistComponent;

  // display elements
  TextButton buttonPanel;
  TextButton sliderPanel;

  // LookAndFeel
  DeckGuiLookAndFeel deckGuiLookAndFeel;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};
