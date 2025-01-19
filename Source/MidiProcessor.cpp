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