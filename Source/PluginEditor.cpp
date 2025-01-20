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
	// Fondo del componente
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

	// Calcular tamaño de fuente proporcional al tamaño del editor
	float fontSize =  getHeight() * 0.05f; // Por ejemplo, 5% de la altura
	g.setColour(juce::Colours::white);
	g.setFont(juce::FontOptions(fontSize));

	// Áreas dinámicas basadas en proporciones del tamaño del editor
	juce::Rectangle<int> area(getWidth() * 0.4, getHeight() * 0.1, getWidth() * 0.8, fontSize);
	juce::Rectangle<int> area2(getWidth() * 0.4, getHeight() * 0.1 + area.getHeight(), getWidth() * 0.8, fontSize);

	// Dibujar texto con las áreas redimensionadas
	g.drawFittedText("Midi Router", area, juce::Justification::centred, 1);

	g.setFont(juce::FontOptions(fontSize * 0.8f));
	g.drawFittedText("by BJR", area2, juce::Justification::centred, 1);

}

void MidiRouterProcessorEditor::resized()
{
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..

	midiDropdownComponent.setBounds(getLocalBounds().reduced(8).removeFromLeft(proportionOfWidth(0.024f)).getX(),
		getLocalBounds().removeFromTop(proportionOfHeight(0.1f)).getBottom(), proportionOfWidth(0.29f), proportionOfHeight(0.1f)
	);
	midiTableComponent.setBounds(getLocalBounds().removeFromBottom(proportionOfHeight(0.78f)).reduced(8));
	presetPanel.setBounds(getLocalBounds().removeFromTop(proportionOfHeight(0.07f)));
}



