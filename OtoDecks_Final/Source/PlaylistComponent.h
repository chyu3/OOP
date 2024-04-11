/*
  ==============================================================================

    PlaylistComponent.h
    Author:  Hsin Yu

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component, public TableListBoxModel, public Button::Listener, public FileDragAndDropTarget, public TextEditor::Listener
{
public:
    PlaylistComponent();
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    int getNumRows() override;
    void paintRowBackground(Graphics & g, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) override;
    
    void buttonClicked(Button * button) override;

    //NEW !!!!!!! Drag and drop feature for playlist
    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;

    
private:
    TableListBox tableComponent;

    //NEW!!!!! Playlist titles, buttons & search feature
    std::vector<std::string> trackTitles;


    AudioFormatManager formatManager;
    DJAudioPlayer player{ formatManager };

    TextEditor searchBox;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};


