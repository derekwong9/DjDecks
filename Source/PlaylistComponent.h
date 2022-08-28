/*
  ==============================================================================

    PlaylistComponent.h
    Created: 13 Aug 2022 8:37:41pm
    Author:  derek

  ==============================================================================
*/

#pragma once
#include <vector>
#include <JuceHeader.h>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include "DJAudioPlayer.h"

//==============================================================================
/*
 */
class PlaylistComponent : public juce::Component,
                          public juce::TableListBoxModel,
                          public juce::Button::Listener
{
public:
  PlaylistComponent(DJAudioPlayer *player1,
                    DJAudioPlayer *player2);
  ~PlaylistComponent() override;

  void paint(juce::Graphics &) override;
  void resized() override;

  int getNumRows() override;
  void paintRowBackground(Graphics &, int rowNumber, int width, int height, bool rowIsSelected) override;
  void paintCell(Graphics &, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
  Component *refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) override;

  void buttonClicked(Button *button) override;

  void addToPlaylist(std::vector<std::string> addTrack, bool writeFile);

  void loadLibrary();
  void updateList();
  bool searchLibrary(std::string searchStr);

private:
  TableListBox tableComponent;

  std::vector<std::vector<std::string>> trackTitles;
  DJAudioPlayer *player1;
  DJAudioPlayer *player2;

  std::string libraryPath;
  void writeToLibrary(std::vector<std::string> addTrack);

  std::vector<std::vector<std::string>> parseCsvFile(std::string inputFileName);
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};
