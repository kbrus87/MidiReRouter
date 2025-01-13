/*
  ==============================================================================

    TableMidiComponent.cpp
    Created: 13 Jan 2025 4:38:25pm
    Author:  Bruno Ramari

  ==============================================================================
*/

#include "TableMidiComponent.h"

MidiTableComponent::MidiTableComponent()
{
    // Configurar columnas de la tabla
    midiTable.getHeader().addColumn("Income MIDI", 1, 200);
    midiTable.getHeader().addColumn("Output MIDI", 2, 200);

    // Hacer visible la tabla
    addAndMakeVisible(midiTable);
    midiTable.setModel(this);
}

void MidiTableComponent::setMidiEvents(const std::vector<MidiEventRow>& events)
{
    midiEventList = events;
    midiTable.updateContent(); // Refrescar la tabla
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
    return midiEventList.size();
}

void MidiTableComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
        g.fillAll(juce::Colours::lightblue);
    else
        g.fillAll(rowNumber % 2 == 0 ? juce::Colours::white : juce::Colours::lightgrey);
}

void MidiTableComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool)
{
    auto text = columnId == 1 ? midiEventList[rowNumber].inputMIDI : midiEventList[rowNumber].outputMIDI;
    g.drawText(text, 2, 0, width - 4, height, juce::Justification::centredLeft);
}