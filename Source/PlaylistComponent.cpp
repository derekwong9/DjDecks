/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 13 Aug 2022 8:37:41pm
    Author:  derek

  ==============================================================================
*/

#include <filesystem>
#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(DJAudioPlayer *_player1,
                                     DJAudioPlayer *_player2) : player1(_player1),
                                                                player2(_player2)
{
  // In your constructor, you should add any child components, and
  // initialise any special settings that your component needs.

  tableComponent.getHeader().addColumn("Track Title", 1, 400);
  tableComponent.getHeader().addColumn("", 2, 200);
  tableComponent.getHeader().addColumn("", 3, 200);

  tableComponent.setModel(this);
  addAndMakeVisible(tableComponent);

  libraryPath = std::filesystem::current_path().string() + "/library.dat";

  loadLibrary();
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint(juce::Graphics &g)
{
  /* This demo code just fills the component's background and
     draws some placeholder text to get you started.

     You should replace everything in this method with your own
     drawing code..
  */

  g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId)); // clear the background
  
  g.setColour(juce::Colours::grey);
  g.drawRect(getLocalBounds(), 1); // draw an outline around the component

  g.setColour(juce::Colours::white);
  g.setFont(14.0f);
  g.drawText("PlaylistComponent", getLocalBounds(),
             juce::Justification::centred, true); // draw some placeholder text
}

void PlaylistComponent::resized()
{
  // This method is where you should set the bounds of any child
  // components that your component contains..
  tableComponent.setBounds(0, 0, getWidth(), getHeight());
}

int PlaylistComponent::getNumRows()
{
  return trackTitles.size();
}

void PlaylistComponent::paintRowBackground(Graphics &g, int rowNumber, int width, int height, bool rowIsSelected)
{
  if (rowIsSelected)
  {
    g.fillAll(Colours::orange);
  }
  else
  {
    g.fillAll(Colours::darkgrey);
  }
}
void PlaylistComponent::paintCell(Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
  g.drawText(trackTitles[rowNumber][0],
             2, 0,
             width - 4, height,
             Justification::centredLeft,
             true);
}

Component *PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate)
{
  if (columnId == 2)
  {
    if (existingComponentToUpdate == nullptr)
    {
      TextButton *btn = new TextButton("Load Left");
      // String id{std::to_string(rowNumber)};
      std::string idString = std::to_string(rowNumber).append("L");
      String id{idString};
      btn->setComponentID(id); // setting the ID to the row number
      btn->addListener(this);
      existingComponentToUpdate = btn;
    }
  }
  if (columnId == 3)
  {
    if (existingComponentToUpdate == nullptr)
    {
      TextButton *btn = new TextButton("Load Right");
      std::string idString = std::to_string(rowNumber).append("R");
      String id{idString};
      btn->setComponentID(id); // setting the ID to the row number
      btn->addListener(this);
      existingComponentToUpdate = btn;
    }
  }

  return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(Button *button)
{
  // we split the numeric side (row number) and the player side (L for left and R for right)
  std::string rawID = button->getComponentID().toStdString();
  if (!rawID.empty())
  {
    char sideID = rawID[rawID.length() - 1];
    rawID.pop_back();
    int numericID = std::stoi(rawID);
    // load the correct player given the side ID
    if (sideID == 'L')
    {
      File path(trackTitles[numericID][1]);
      player1->loadURL(URL{path});
    }
    else if (sideID == 'R')
    {
      File path{trackTitles[numericID][1]};
      player2->loadURL(URL{path});
    }
    else
    {
      DBG("PlaylistComponent::buttonClicked sideID is not R or L");
    }
  }
  else
  {
    DBG("PlaylistComponent::buttonClicked rawID is empty");
  }
}

void PlaylistComponent::addToPlaylist(std::vector<std::string> addTrack, bool writeFile)
{
  trackTitles.push_back(addTrack);
  tableComponent.updateContent();

  if (writeFile)
    writeToLibrary(addTrack);
}

void PlaylistComponent::writeToLibrary(std::vector<std::string> addTrack)
{ // write to file
  std::ofstream outFile(libraryPath, std::ios_base::app);
  if (outFile.is_open())
  {
    DBG("PlaylistComponent::addToPlaylist library file exists, preparing to write");
    outFile << addTrack[0] << "," << addTrack[1] << std::endl;
  }
  else
  {
    DBG("PlaylistComponent::addToPlaylist library file failed to open for output");
  }
  outFile.close();
}

void PlaylistComponent::loadLibrary()
{
  // load the library to the trackTitles
  trackTitles.clear();

  std::ifstream libraryFile(libraryPath);

  if (libraryFile.is_open())
  {
    DBG("PlaylistComponent::loadLibrary library.dat DOES exist, loading content");
    std::vector<std::vector<std::string>> resultLoad = parseCsvFile(libraryPath);
    for (int i = 0; i < resultLoad.size(); ++i)
    {
      addToPlaylist(resultLoad[i], false);
    }
  }
  else
  {
    DBG("PlaylistComponent::loadLibrary library.dat DOES NOT exist, creating one");
    std::ofstream(libraryPath).flush();
  }

  updateList();
}

std::vector<std::vector<std::string>> PlaylistComponent::parseCsvFile(std::string inputFileName)
{
  std::vector<std::vector<std::string>> content;
  std::vector<std::string> row;
  std::string line, word;

  std::fstream file(inputFileName, std::ios::in);
  if (file.is_open())
  {
    while (getline(file, line))
    {
      row.clear();

      std::stringstream str(line);

      while (std::getline(str, word, ','))
      {
        row.push_back(word);
      }

      content.push_back(row);
    }
  }
  else
    std::cout << "Could not open the file\n";

  return content;
}

bool PlaylistComponent::searchLibrary(std::string searchStr)
{
  std::vector<std::vector<std::string>> searchResults;
  for (int i = 0; i < trackTitles.size(); ++i)
  {
    std::size_t found = trackTitles[i][0].find(searchStr);
    if (found != std::string::npos)
    {
      DBG("PlaylistComponent::searchLibrary found " << trackTitles[i][0]);
      searchResults.push_back(trackTitles[i]);
    }
  }

  if (searchResults.size() > 0)
  {
    trackTitles = searchResults;
    updateList();
    return true;
  }
  else
  {
    return false;
  }
}

void PlaylistComponent::updateList()
{
  tableComponent.updateContent();
  tableComponent.repaint();
}