/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MidiRouterProcessorEditor::MidiRouterProcessorEditor(MidiRouterProcessor& p)
	: AudioProcessorEditor(&p), audioProcessor(p), midiProcessor(p.getMidiProcessor()), midiTableComponent(midiProcessor.translationTable, std::bind(&MidiProcessor::setOutputMidi, &midiProcessor, std::placeholders::_1, std::placeholders::_2))
{
	//midiProcessor.addChangeListener(&midiTableComponent);
	midiProcessor.addListener(&midiTableComponent);

	midiTableComponent.setMidiEvents(midiProcessor.translationTable);

	midiDropdownComponent.setMidiEvents(constMidiEventList);
	midiDropdownComponent.onChange(midiProcessor);

	saveButton.setButtonText("Save Preset");
	saveButton.setColour(2, juce::Colour::fromRGB(23, 34, 125));
	loadButton.setButtonText("Load Preset");
	loadButton.setColour(4, juce::Colour::fromRGB(23, 34, 125));
	loadButton.repaint();
	saveButton.repaint();

	addAndMakeVisible(midiTableComponent);
	addAndMakeVisible(midiDropdownComponent);
	addAndMakeVisible(saveButton);
	addAndMakeVisible(loadButton);
	DBG(midiTableComponent.getWidth());
	setSize(midiTableComponent.getWidth(), 500);
}

MidiRouterProcessorEditor::~MidiRouterProcessorEditor()
{
	//midiProcessor.removeAllChangeListeners();
	midiProcessor.removeAllListener();
}

//==============================================================================
void MidiRouterProcessorEditor::paint(juce::Graphics& g)
{
	// (Our component is opaque, so we must completely fill the background with a solid colour)
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

	g.setColour(juce::Colours::white);
	g.setFont(juce::FontOptions(25.0f));

	juce::Rectangle<int> area(50, 20, 500, 50);
	juce::Rectangle<int> area2(50, 60, 500, 50);
	juce::Rectangle<int> area3(240, 70, 250, 50);

	g.drawFittedText("Midi Router", area, juce::Justification::centred, 1);
	g.drawFittedText("by BJR", area2, juce::Justification::centred, 1);

}

void MidiRouterProcessorEditor::resized()
{
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..

	midiDropdownComponent.setBounds(10, 40, 125, 50);
	midiTableComponent.setBounds(10, 110, 400, 500);
	saveButton.setBounds(10, 10, 70, 18);
	loadButton.setBounds(90, 10, 70, 18);
}



