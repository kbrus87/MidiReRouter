/*
  ==============================================================================

	MidiProcessor.cpp
	Created: 16 Jan 2025 8:23:02pm
	Author:  Bruno Ramari

  ==============================================================================
*/

#include "MidiProcessor.h"
MidiProcessor::MidiProcessor() {
	setTranslationMap();
}

void MidiProcessor::setTranslationMap() {
	for (auto i : translationTable) {
		numberToName.count(i.outputMIDInumber) > 0 ? translationMap[i.inputMIDInumber] = i.outputMIDInumber : translationMap[i.inputMIDInumber] = i.inputMIDInumber;
	}
}

void MidiProcessor::process(juce::MidiBuffer& midiMessages)
{
	processedBuffer.clear();
	juce::MidiBuffer::Iterator it(midiMessages); // Iterador para recorrer el buffer
	juce::MidiMessage currentMessage;
	int samplePos;

	while (it.getNextEvent(currentMessage, samplePos))
	{
		if (currentMessage.isController())
		{

		}

		if (currentMessage.isNoteOnOrOff())
		{

			if (translationMap.count(currentMessage.getNoteNumber()) > 0) {
				currentMessage.setNoteNumber(translationMap.at(currentMessage.getNoteNumber()));
			}
		}

		processedBuffer.addEvent(currentMessage, samplePos);

	}
	midiMessages.swapWith(processedBuffer);
}

void MidiProcessor::setTranslationTable(TranslationMidiTable table) {
	translationTable = table;

	//sendChangeMessage();
	notify("translationMidiTable", table);
}

void MidiProcessor::setTranslationMap(std::map<int, int> map) {
	translationMap = map;
}

const TranslationMidiTable MidiProcessor::getTranslationTable() const {
	return translationTable;
}

void MidiProcessor::setOutputMidi(int index, juce::String midiName)
{
	std::string note = midiName.toStdString();

	std::regex midiNoteRegex(midiNameRegex);
	bool isNoteName = std::regex_match(note, midiNoteRegex);

	translationTable[index].outputMIDI = midiName;
	if (isNoteName) {
		try {
			translationTable[index].outputMIDInumber = nameToNumber.at(note);
		}
		catch (const std::out_of_range& oor) {
			DBG("MidiNota no encontrada " << midiName);
		}
	}
	setTranslationMap();
}

juce::ValueTree MidiProcessor::translationMapToValueTree() {
	juce::ValueTree mapTree("TranslationMap");
	for (const auto& item : translationMap) {
		juce::ValueTree mapEntry("Entry");
		mapEntry.setProperty("Key", item.first, nullptr);
		mapEntry.setProperty("Value", item.second, nullptr);
		mapTree.addChild(mapEntry, -1, nullptr);
	}
	return mapTree;
}

juce::ValueTree MidiProcessor::translationTableToValueTree() {
	juce::ValueTree vectorTree("TranslationTable");
	for (const auto& item : translationTable) {
		juce::ValueTree vectorEntry("Entry");
		vectorEntry.setProperty("inputMIDInumber", item.inputMIDInumber, nullptr);
		vectorEntry.setProperty("inputMIDI", item.inputMIDI, nullptr);
		vectorEntry.setProperty("outputMIDI", item.outputMIDI, nullptr);
		vectorEntry.setProperty("outputMIDInumber", item.outputMIDInumber, nullptr);

		vectorTree.addChild(vectorEntry, -1, nullptr);
	}
	return vectorTree;
}

void MidiProcessor::loadTranslationMapFromValueTree(juce::ValueTree mapTree) {

	std::map<int, int> translationMap;
	TranslationMidiTable translationTableV;
	for (int i = 0; i < mapTree.getNumChildren(); ++i) {
		auto entry = mapTree.getChild(i);

		// Obtener las propiedades del ValueTree
		int key = entry.getProperty("Key");
		int value = entry.getProperty("Value");

		// Actualizar el mapa
		translationMap[key] = value;

		// Crear la fila y añadirla al vector
		MidiTranslationRow row = {
			key,
			juce::MidiMessage::getMidiNoteName(key, true, true, 4),
			juce::MidiMessage::getMidiNoteName(value, true, true, 4),
			value
		};
		translationTableV.push_back(row);
	}
	translationMap = translationMap;
	translationTable = translationTableV;

	notify("translationMidiTable", translationTable);
}

void MidiProcessor::loadTranslationTableFromValueTree(juce::ValueTree mapTree) {

	TranslationMidiTable translationTableV;
	for (int i = 0; i < mapTree.getNumChildren(); ++i) {
		auto vectorEntry = mapTree.getChild(i);
		MidiTranslationRow value = { vectorEntry.getProperty("inputMIDInumber"), vectorEntry.getProperty("inputMIDI"), vectorEntry.getProperty("outputMIDI"),  vectorEntry.getProperty("outputMIDInumber") };
		translationTableV.push_back(value);
	}
	translationTable = translationTableV;
	notify("translationMidiTable", translationTable);
}