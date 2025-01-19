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

	addAndMakeVisible(midiTableComponent);
	addAndMakeVisible(midiDropdownComponent);

	setSize(midiTableComponent.getWidth() + midiDropdownComponent.getWidth() + 200, 600);
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
	g.setFont(juce::FontOptions(15.0f));

	juce::Rectangle<int> area(10, 10, 300, 50);

	g.drawFittedText("Hello mami!", area, juce::Justification::centred, 1);
}

void MidiRouterProcessorEditor::resized()
{
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..

	midiDropdownComponent.setBounds(10, 30, 125, 50);
	midiTableComponent.setBounds(50, 100, 400, 500);
}



