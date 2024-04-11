/*
  ==============================================================================

    DeckGUI.cpp
    Author:  Hsin Yu

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================

const Colour themeBackgroundColor = Colour(0xff2b2b2b);
const Colour themeButtonColor = Colour(0xff007acc);
const Colour themeSliderColor = Colour(0xff007acc);
const Colour themeTextColor = Colours::white;


DeckGUI::DeckGUI(DJAudioPlayer* _player, 
                AudioFormatManager & 	formatManagerToUse,
                AudioThumbnailCache & 	cacheToUse
           ) : player(_player), 
               waveformDisplay(formatManagerToUse, cacheToUse)
{

    addAndMakeVisible(playButton);
    playButton.addListener(this);

    addAndMakeVisible(stopButton);
    stopButton.addListener(this);

    addAndMakeVisible(loadButton);
    loadButton.addListener(this);
    
    //Volume Slider   
    addAndMakeVisible(volSlider);
    volSlider.addListener(this);
    volSlider.setValue(0.5); //default volume 50%
    addAndMakeVisible(volLabel);
    volLabel.setText("Volume", juce::dontSendNotification);
    volLabel.attachToComponent(&volSlider, false);
    volLabel.setJustificationType(juce::Justification::centred);

    //Speed Slider
    addAndMakeVisible(speedSlider);
    speedSlider.addListener(this);
    speedSlider.setValue(1); //defaut speed at 1 (50%)
    //addAndMakeVisible(speedLabel);
    //speedLabel.setText("Speed", juce::dontSendNotification);
    //speedLabel.attachToComponent(&speedSlider, false);


    //Position Slider
    addAndMakeVisible(posSlider);
    posSlider.addListener(this);
    //addAndMakeVisible(posLabel);
    //posLabel.setText("Position", juce::dontSendNotification);
    //posLabel.attachToComponent(&posSlider, false);

    addAndMakeVisible(waveformDisplay);


 //NEW!!!!! set new color theme for button and slider
    playButton.setColour(TextButton::buttonColourId, themeButtonColor);
    stopButton.setColour(TextButton::buttonColourId, themeButtonColor);
    loadButton.setColour(TextButton::buttonColourId, themeButtonColor);

    volSlider.setColour(Slider::thumbColourId, themeSliderColor);
    speedSlider.setColour(Slider::thumbColourId, themeSliderColor);
    posSlider.setColour(Slider::thumbColourId, themeSliderColor);


 //NEW!!!!! 
    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.5, 2, 0);
    posSlider.setRange(0.0, 1.0);

    startTimer(500);


}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    //NEW!!!!! setting background and text color 
    g.fillAll(themeBackgroundColor);
    g.setColour(themeTextColor);
    
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 8; 
    playButton.setBounds(0, 0, getWidth(), rowH - 20);
    stopButton.setBounds(0, rowH, getWidth(), rowH - 20);  

    volSlider.setBounds(0, rowH * 2, getWidth(), rowH);
    volLabel.setBounds(0, rowH * 2 - 30, getWidth(), rowH); //label above the volume slider

    speedSlider.setBounds(0, rowH * 3, getWidth(), rowH);
    //speedLabel.setBounds(0, rowH * 3 - 25, getWidth(), rowH); //label above the speed slider

    posSlider.setBounds(0, rowH * 4, getWidth(), rowH);
    //posLabel.setBounds(0, rowH * 4 - 20, getWidth(), rowH); //label above the position slider

    waveformDisplay.setBounds(0, rowH * 5, getWidth(), rowH * 2);
    loadButton.setBounds(0, rowH * 7, getWidth(), rowH - 20);

    
}

void DeckGUI::buttonClicked(Button* button)
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
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
        {
            player->loadURL(URL{chooser.getResult()});
            // and now the waveformDisplay as well
            waveformDisplay.loadURL(URL{chooser.getResult()}); 
        });
    }

}

void DeckGUI::sliderValueChanged (Slider *slider)
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
    
}

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
  return true; 
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
  std::cout << "DeckGUI::filesDropped" << std::endl;
  if (files.size() == 1)
  {
    player->loadURL(URL{File{files[0]}});
  }
}

void DeckGUI::timerCallback()
{
    //std::cout << "DeckGUI::timerCallback" << std::endl;
    waveformDisplay.setPositionRelative(
            player->getPositionRelative());
}


    

