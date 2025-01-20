/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MidiRouterProcessorEditor::MidiRouterProcessorEditor(MidiRouterProcessor& p)
	: AudioProcessorEditor(&p), audioProcessor(p), midiProcessor(p.getMidiProcessor()), 
	midiTableComponent(midiProcessor.translationTable, std::bind(&MidiProcessor::setOutputMidi, &midiProcessor, std::placeholders::_1, std::placeholders::_2)),
	presetPanel(p.getPresetManager())
{
	//midiProcessor.addChangeListener(&midiTableComponent);
	midiProcessor.addListener(&midiTableComponent);
	p.getPresetManager().addListener(&presetPanel);

	midiTableComponent.setMidiEvents(midiProcessor.translationTable);

	midiDropdownComponent.setMidiEvents(constMidiEventList);
	midiDropdownComponent.onChange(midiProcessor);

	addAndMakeVisible(midiTableComponent);
	addAndMakeVisible(midiDropdownComponent);
	addAndMakeVisible(presetPanel);

	setResizable(true, true);
	setSize(midiTableComponent.getWidth(), 500);
}

MidiRouterProcessorEditor::~MidiRouterProcessorEditor()
{
	//midiProcessor.removeAllChangeListeners();
	midiProcessor.removeAllListener();
	audioProcessor.getPresetManager().removeAllListener();
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
	DBG(getLocalBounds().getX() << " "<< getLocalBounds().getY()<<" "<<getLocalBounds().getWidth()<<" " << getLocalBounds().getHeight());
	// midiDropdownComponent.setBounds(10, 40, 125, 50);
	midiDropdownComponent.setBounds(getLocalBounds().removeFromLeft(proportionOfWidth(0.024f)).getX(),
		getLocalBounds().removeFromTop(proportionOfHeight(0.1f)).getBottom(), proportionOfWidth(0.29f), proportionOfHeight(0.1f)
	);
	midiTableComponent.setBounds(getLocalBounds().removeFromBottom(proportionOfHeight(0.78f)));
	presetPanel.setBounds(getLocalBounds().removeFromTop(proportionOfHeight(0.07f)));
}



