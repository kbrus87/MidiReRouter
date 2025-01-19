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
			DBG("es Controlador: "; currentMessage.isController());
			DBG("controllerNumber: "; currentMessage.getControllerNumber());
			DBG(currentMessage.getControllerNumber());
			DBG("value: "; currentMessage.getControllerValue());
			DBG(currentMessage.getControllerValue());
			DBG("controller");
		}

		// Si es un evento Note On/Off, modifica el número de nota


		if (currentMessage.isNoteOnOrOff())
		{
			DBG("eS NOTA");
			DBG("input nota name: " << currentMessage.getDescription());
			DBG("input nota number: " << juce::String(currentMessage.getNoteNumber()));
			if (translationMap.count(currentMessage.getNoteNumber())>0) {
				// Si la clave existe
				DBG("output nota number: " << translationMap.at(currentMessage.getNoteNumber()));
				currentMessage.setNoteNumber(translationMap.at(currentMessage.getNoteNumber()));
			}
			else {
				// La clave no existe
				DBG("Nota no encontrada en el mapa");
				// Maneja el caso donde no encuentres la clave, si es necesario
			}
		}

		// Agrega el mensaje al buffer procesado
		processedBuffer.addEvent(currentMessage, samplePos);

	}
	midiMessages.swapWith(processedBuffer);
}

void MidiProcessor::setTranslationTable(TranslationMidiTable table) {
	translationTable = table;

	//sendChangeMessage();
	notify("translationMidiTable", table);
}

const TranslationMidiTable MidiProcessor::getTranslationTable() const {
	return translationTable;
}

void MidiProcessor::setOutputMidi(int index, juce::String midiName)
{
	std::string note = midiName.toStdString();

	std::regex midiNoteRegex("^(C|D|E|F|G|A|B)(#?)([0-9])$");
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