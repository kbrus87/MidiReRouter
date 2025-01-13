/*
  ==============================================================================

    TableMidiComponent.h
    Created: 13 Jan 2025 4:35:43pm
    Author:  Bruno Ramari

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "types.h"  // Incluir tu estructura para MidiEvent

class MidiTableComponent : public juce::Component, private juce::TableListBoxModel
{
public:
    MidiTableComponent();

    void paint(juce::Graphics&) override;
    void resized() override;

    // Cargar datos en la tabla
    void setMidiEvents(const std::vector<MidiEventRow>& events);

private:
    juce::TableListBox midiTable;
    std::vector<MidiEventRow> midiEventList;

    // Métodos para manejar el contenido de la tabla
    int getNumRows() override;
    void paintRowBackground(juce::Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(juce::Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiTableComponent)
};