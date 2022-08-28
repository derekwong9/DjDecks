/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public AudioAppComponent,
                      public Label::Listener,
                      public Button::Listener
{
public:
  //==============================================================================
  MainComponent();
  ~MainComponent();

  //==============================================================================
  void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
  void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
  void releaseResources() override;

  //==============================================================================
  void paint(Graphics &g) override;
  void resized() override;
  /** implement Label::Listener */
  void labelTextChanged(Label *labelThatHasChanged) override;
  /** implement Button::Listener */
  void buttonClicked(Button *) override;

private:
  //==============================================================================
  // Your private member variables go here...

  AudioFormatManager formatManager;
  AudioThumbnailCache thumbCache{100};

  DJAudioPlayer player1{formatManager};
  DJAudioPlayer player2{formatManager};

  PlaylistComponent playlistComponent{&player1, &player2};

  DeckGUI deckGUI1{&player1, formatManager, thumbCache, &playlistComponent};
  DeckGUI deckGUI2{&player2, formatManager, thumbCache, &playlistComponent};

  MixerAudioSource mixerSource;
  Label searchBar;
  TextButton resetSearchBar{"Reset Search"};

  void initSearchBar();
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
