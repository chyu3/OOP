/*
  ==============================================================================

    PlaylistComponent.cpp
    Author:  Hsin Yu

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================

const Colour themeBackgroundColor = Colour(0xff2b2b2b);
const Colour themeButtonColor = Colour(0xff007acc);
const Colour themeTextColor = Colours::white;


PlaylistComponent::PlaylistComponent()
{
    //Trial Tracks
    /*trackTitles.push_back("Track 1");
    trackTitles.push_back("Track 2");
    trackTitles.push_back("Track 3");*/
   
    
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    tableComponent.getHeader().addColumn("Track title: Drag files below to library", 1, 400);
    tableComponent.getHeader().addColumn("Play", 2, 100);

    //reserve storage for playlist capacity
    trackTitles.reserve(100);

 //NEW!!!!!! New button/functions for the playlist
    tableComponent.getHeader().addColumn("Upload Disk 1", 3, 100);
    tableComponent.getHeader().addColumn("Upload Disk 2", 4, 100);
    tableComponent.getHeader().addColumn("Cancel", 5, 100);
    
    tableComponent.setModel(this);
    
    addAndMakeVisible(tableComponent);

 //NEW!!!!! Set Table colours
    tableComponent.setColour(TableListBox::outlineColourId, themeButtonColor);

 //NEW!!!!! Add Search Box
    addAndMakeVisible(searchBox);
    searchBox.addListener(this);
    searchBox.setText("Search here...", juce::dontSendNotification);
   
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    //NEW !!!!!! set playlist colour
    g.fillAll(themeBackgroundColor);
    g.setColour(themeTextColor);
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    tableComponent.setBounds(0, 0, getWidth(), getHeight());

    //NEW!!!!! Search box layout
    searchBox.setBounds(10, getHeight() - 20, getWidth() - 20, 20);

}

int PlaylistComponent::getNumRows(){
    return static_cast<int>(trackTitles.size());
}
void PlaylistComponent::paintRowBackground(Graphics & g, int rowNumber, int width, int height, bool rowIsSelected){
    if (rowIsSelected) {
        g.fillAll(Colours::orange);
    }else{
        g.fillAll(Colours::darkgrey);
    }
}
void PlaylistComponent::paintCell(Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected){
    g.drawText(trackTitles[rowNumber],2,0,width-4, height, Justification::centredLeft,true);
}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) {
    if(columnId == 2){
        if(existingComponentToUpdate==nullptr){
            TextButton* playBtn = new TextButton{"play"};
            String id{std::to_string(rowNumber)};
            playBtn->setComponentID(id);
            
            playBtn->addListener(this);
            existingComponentToUpdate=playBtn;
        }
    }

    //NEW!!!!! upload to disk 1 button
    if (columnId == 3) {
        if (existingComponentToUpdate == nullptr) {
            TextButton* upload1Btn = new TextButton{ "-> 1" };
            String id{ std::to_string(rowNumber) };
            upload1Btn->setComponentID(id);

            upload1Btn->addListener(this);
            existingComponentToUpdate = upload1Btn;
        }
    }

    //NEW!!!!! upload to disk 2 button
    if (columnId == 4) {
        if (existingComponentToUpdate == nullptr) {
            TextButton* upload2Btn = new TextButton{ "-> 2" };
            String id{ std::to_string(rowNumber) };
            upload2Btn->setComponentID(id);

            upload2Btn->addListener(this);
            existingComponentToUpdate = upload2Btn;
        }
    }

    //NEW!!!!! Cancel button
    if (columnId == 5) {
        if (existingComponentToUpdate == nullptr) {
            TextButton* cancelBtn = new TextButton{ "X" };
            String id{ std::to_string(rowNumber) };
            cancelBtn->setComponentID(id);

            cancelBtn->addListener(this);
            existingComponentToUpdate = cancelBtn;
        }
    }

    return existingComponentToUpdate;
}

//NEW!!!!! 
void PlaylistComponent::buttonClicked(Button * button) {
    int id = std::stoi(button->getComponentID().toStdString());

    //Play Button
    if (button->getName() == "play") {
        std::cout << "PlaylistComponent::buttonClicked trackTitles id: " << trackTitles[id] << std::endl;

        //retrieve audio file to play
        if (id >= 0 && id < trackTitles.size()) {
            String audioTitle = String(trackTitles[id].c_str());
            File audioFile = File::getCurrentWorkingDirectory().getChildFile(audioTitle + ".mp3");

            //Failed to play file chosen
            //construct URL using path name
            URL audioURL = audioFile.getFullPathName();
            player.loadURL(audioURL);
            player.playAudio(audioURL);
            
        }
    }

    //Cancel Button
    else if (button->getName() == "X") {
        std::cout << "PlaylistComponent::buttonClicked trackTitles id: " << trackTitles[id] << std::endl;

        //remove track from playlist
        if (id >= 0 && id < trackTitles.size()) {
            trackTitles.erase(trackTitles.begin() + id);

            //update table change
            tableComponent.updateContent();
        }
    }
   
}

bool PlaylistComponent::isInterestedInFileDrag(const juce::StringArray& files) {
    return true;
}


//NEW!!!!! Success attempt at droping labeled audio files
void PlaylistComponent::filesDropped(const juce::StringArray& files, int x, int y) {
    //test 1: successful
    for (auto file : files) {
        std::cout << "Dropped file: " << file << std::endl;

        //add file to playlist
        File audioFile(file);
        String audioTitle = audioFile.getFileNameWithoutExtension();

        //convert String to std::string
        std::string stdAudioTitle = audioTitle.toStdString();

        //display audio title
        trackTitles.push_back(stdAudioTitle);

        tableComponent.updateContent();
        
    }


}


