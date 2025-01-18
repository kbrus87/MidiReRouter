/*
  ==============================================================================

	MidiProcessor.h
	Created: 7 Jan 2025 4:00:57pm
	Author:  Bruno Ramari

  ==============================================================================
*/

#pragma once

#include <juce_audio_basics/juce_audio_basics.h> // Incluye las clases MidiBuffer y MidiMessage
#include <juce_core/juce_core.h> 
#include "JuceHeader.h"
#include "types.h"

class MidiProcessor
{
public:

	void process(juce::MidiBuffer& midiMessages);
	void setTranslationTable(TranslationMidiTable table);
	TranslationMidiTable getTranslationTable();


private:

	TranslationMidiTable translationTable = { {36, "C2", "C1", 24} };
	juce::MidiBuffer processedBuffer;
};