/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p)
	: AudioProcessorEditor(&p), audioProcessor(p)
{
	midiTableComponent.setMidiEvents(midiEventList);
    midiDropdownComponent.setMidiEvents(midiEventList);
    addAndMakeVisible(midiTableComponent);
    addAndMakeVisible(midiDropdownComponent);

    setSize(midiTableComponent.getWidth() + midiDropdownComponent.getWidth() + 20, 600);
       

}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
}

//==============================================================================
void NewProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));

    juce::Rectangle<int> area(10, 10, 300, 50);

    g.drawFittedText ("Hello mami!", area, juce::Justification::centred, 1);
}

void NewProjectAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    midiTableComponent.setBounds(50,100,250,500);
    midiDropdownComponent.setBounds(10,30,125,50);
}



