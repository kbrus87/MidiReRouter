/*
  ==============================================================================

	TableMidiComponent.cpp
	Created: 13 Jan 2025 4:38:25pm
	Author:  Bruno Ramari

  ==============================================================================
*/

#include "TableMidiComponent.h"

MidiTableComponent::MidiTableComponent(TranslationMidiTable& events)
	: midiEventList(events) // Inicialización de la referencia
{
	// Hacer visible la tabla

	addAndMakeVisible(midiTable);
	midiTable.setModel(this);

	// Configurar columnas de la tabla
	midiTable.getHeader().addColumn("Income MIDI", 1, 200);
	midiTable.getHeader().addColumn("Output MIDI", 2, 200);


	setSize(400, 300);
}

MidiTableComponent::~MidiTableComponent(){}

void MidiTableComponent::setMidiEvents(TranslationMidiTable& events)
{
	midiEventList = events;
	midiTable.updateContent(); // Refrescar la tabla
}

std::vector<MidiTranslationRow> MidiTableComponent::getMidiEvents() {
	return midiEventList;
}

void MidiTableComponent::paint(juce::Graphics& g)
{
	g.fillAll(juce::Colours::darkgrey);
}

void MidiTableComponent::resized()
{
	midiTable.setBounds(getLocalBounds());
}

int MidiTableComponent::getNumRows()
{
	try {
	return midiEventList.size();
	}
	catch (std::exception e) {
		DBG(e.what());
		return 0;
	}
}

void MidiTableComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
	if (rowIsSelected)
		g.fillAll(juce::Colours::lightblue);
	else
		g.fillAll(rowNumber % 2 == 0 ? juce::Colours::white : juce::Colours::lightgrey);
}

void MidiTableComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
	if (rowNumber < midiEventList.size())
	{
		const auto& row = midiEventList[rowNumber];
		juce::String text;

		if (columnId == 1)      text = row.inputMIDI;
		else if (columnId == 2) text = row.outputMIDI;

		g.drawText(text, 2, 0, width - 4, height, juce::Justification::centredLeft);
	}
}


void MidiTableComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
	midiTable.updateContent();
}