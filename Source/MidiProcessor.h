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
#include <regex>
#include "JuceHeader.h"
#include "types.h"
#include "TableMidiComponent.h"
#include "Broadcaster.h"
#include "constants.h"

class MidiProcessor : public Broadcaster //, public juce::ChangeBroadcaster
{
public:
	MidiProcessor();

	void process(juce::MidiBuffer& midiMessages);

	void setTranslationTable(TranslationMidiTable table);
	void setTranslationMap(std::map<int, int> map);


	// translationMap modifiers;
	TranslationMidiTable addTranslationBlock(const MidiTranslationRow& descriptor = MidiTranslationRow());
	TranslationMidiTable removeTranslationBlock(int index);
	void clearTranslationTable();
	TranslationMidiTable modifyTranslationRow(const MidiTranslationRow& descriptor = MidiTranslationRow());

	// Sets the output/input midi note for a block by text name
	void setOutputMidi(int index, juce::String midiName);
	void setInputMidi(int index, juce::String midiName);
	void setInputFantasyName(int index, juce::String inputFantasyName);
	void setOutputFantasyName(int index, juce::String outputFantasyName);


	const TranslationMidiTable getTranslationTable() const; //;


	const std::map<int, int>* getTranslationMapPointer() const { return &translationMap; };
	const std::map<int, int> getTranslationMap() const { return translationMap; };

	juce::ValueTree translationMapToValueTree();
	juce::ValueTree translationTableToValueTree();

	void loadTranslationMapFromValueTree(juce::ValueTree mapTree);
	void loadTranslationTableFromValueTree(juce::ValueTree mapTree);

	void loadInputMap(std::vector<MapElement> inputMap);

	TranslationMidiTable translationTable = { {1, 48, "C2", "C1", 36, true} };


private:
	std::map<int, int> translationMap;
	juce::MidiBuffer processedBuffer;
	juce::WebBrowserComponent webView;

	void updateTranslationMap();
	int nextTranslationMapIndex();
};