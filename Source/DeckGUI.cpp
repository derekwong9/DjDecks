/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer *_player,
                 AudioFormatManager &formatManagerToUse,
                 AudioThumbnailCache &cacheToUse,
                 PlaylistComponent *_playlistComponent) : player(_player),
                                                          waveformDisplay(formatManagerToUse, cacheToUse),
                                                          playlistComponent(_playlistComponent)

{
    // active buttons
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(addToListButton);
    // sliders
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(filterFreqSlider);
    addAndMakeVisible(filterResSlider);
    // waveform display
    addAndMakeVisible(waveformDisplay);

    // Listeners
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);
    addToListButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    filterFreqSlider.addListener(this);
    filterResSlider.addListener(this);
    // Ranges
    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 3);
    posSlider.setRange(0.0, 1.0);
    filterFreqSlider.setRange(20.0f, 20000.0f);
    filterResSlider.setRange(0.1f, 1.0f);

    // default values
    volSlider.setValue(1);
    speedSlider.setValue(0);
    filterFreqSlider.setValue(20.0f);
    filterResSlider.setValue(0.1f);

    // display elements
    buttonPanel.setColour(TextButton::buttonColourId, Colours::dodgerblue);
    sliderPanel.setColour(TextButton::buttonColourId, Colours::dodgerblue);
    addAndMakeVisible(buttonPanel);
    addAndMakeVisible(sliderPanel);
    // timer
    startTimer(500);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint(Graphics &g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId)); // clear the background

    g.setColour(Colours::grey);
    g.drawRect(getLocalBounds(), 1); // draw an outline around the component

    g.setColour(Colours::white);
    g.setFont(14.0f);
}

void DeckGUI::resized()
{
    // initialize bounds
    auto area = getLocalBounds();
    // relative size settings
    int largeHeight = getHeight() / 3;
    int smallHeight = getHeight() / 6;
    int buttonWidth = getWidth() / 4;
    int sliderWidth = getWidth() / 4;

    // waveform display
    waveformDisplay.setBounds(area.removeFromTop(largeHeight));

    // position slider
    posSlider.setBounds(area.removeFromTop(smallHeight));

    // buttons
    Rectangle buttonPanelArea = area.removeFromTop(smallHeight);

    // buttonPanel.setBounds(buttonPanelArea);
    playButton.setBounds(buttonPanelArea.removeFromLeft(buttonWidth));
    stopButton.setBounds(buttonPanelArea.removeFromLeft(buttonWidth));
    loadButton.setBounds(buttonPanelArea.removeFromLeft(buttonWidth));
    addToListButton.setBounds(buttonPanelArea.removeFromLeft(buttonWidth));

    // slider panel
    Rectangle sliderPanelArea = area.removeFromTop(largeHeight);
    speedSlider.setBounds(sliderPanelArea.removeFromLeft(sliderWidth));
    volSlider.setBounds(sliderPanelArea.removeFromLeft(sliderWidth));
    filterFreqSlider.setBounds(sliderPanelArea.removeFromLeft(sliderWidth));
    filterResSlider.setBounds(sliderPanelArea.removeFromLeft(sliderWidth));
}

void DeckGUI::buttonClicked(Button *button)
{
    if (button == &playButton)
    {
        std::cout << "Play button was clicked " << std::endl;
        player->start();
    }
    if (button == &stopButton)
    {
        std::cout << "Stop button was clicked " << std::endl;
        player->stop();
    }
    if (button == &loadButton)
    {
        auto fileChooserFlags =
            FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser &chooser)
                             {
            player->loadURL(URL{chooser.getResult()});
            // and now the waveformDisplay as well
            waveformDisplay.loadURL(URL{chooser.getResult()}); });
    }
    if (button == &addToListButton)
    {
        auto fileChooserFlags =
            FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser &chooser)
                             {
                                 // juce::File chooserResults = chooser.getResult();
                                 std::vector<std::string> result(2);
                                 juce::File chooserResult = chooser.getResult();
                                 std::cout << "file name " << chooserResult.getFileName().toStdString() << std::endl;

                                 result[0] = chooserResult.getFileName().toStdString();
                                 result[1] = chooserResult.getFullPathName().toStdString();
                                 playlistComponent->addToPlaylist(result, true); });
    }
}

void DeckGUI::sliderValueChanged(Slider *slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }

    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
    if (slider == &filterFreqSlider)
    {
        player->setFilterFreq(slider->getValue());
    }
    if (slider == &filterResSlider)
    {
        player->setFilterRes(slider->getValue());
    }
}

bool DeckGUI::isInterestedInFileDrag(const StringArray &files)
{
    std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
    return true;
}

void DeckGUI::filesDropped(const StringArray &files, int x, int y)
{
    std::cout << "DeckGUI::filesDropped" << std::endl;
    if (files.size() == 1)
    {
        player->loadURL(URL{File{files[0]}});
    }
}

void DeckGUI::timerCallback()
{
    // std::cout << "DeckGUI::timerCallback" << std::endl;

    if (player->currentURL != waveformDisplay.currentURL)
    {
        waveformDisplay.loadURL(player->currentURL);
    }

    waveformDisplay.setPositionRelative(
        player->getPositionRelative());
}