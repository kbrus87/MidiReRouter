/*
  ==============================================================================

	MidiProcessor.cpp
	Created: 16 Jan 2025 8:23:02pm
	Author:  Bruno Ramari

  ==============================================================================
*/

#include "MidiProcessor.h"
MidiProcessor::MidiProcessor() {
	updateTranslationMap();
}

void MidiProcessor::updateTranslationMap() {
	translationMap.clear();
	for (const auto& i : translationTable) {
		juce::String isActive = i.active ? "true" : "false";
		if (i.active) {
			 translationMap[i.inputMIDInumber] = numberToName.count(i.outputMIDInumber) > 0 ? i.outputMIDInumber : i.inputMIDInumber;
		}
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

		if (currentMessage.isNoteOnOrOff())
		{
			const int inputNote = currentMessage.getNoteNumber();

			if (translationMap.count(inputNote) > 0)
			{
				if (triggerMultiple)
				{
					auto range = translationMap.equal_range(inputNote);
					for (auto it = range.first; it != range.second; ++it)
					{
						juce::MidiMessage newMessage = currentMessage;
						newMessage.setNoteNumber(it->second);
						processedBuffer.addEvent(newMessage, samplePos);
					}
				}
				else
				{
					juce::MidiMessage newMessage = currentMessage;
					newMessage.setNoteNumber(translationMap.at(inputNote)); // toma el primero
					processedBuffer.addEvent(newMessage, samplePos);
				}
			}
			else
			{
				if (passThroughEnabled)
				{
					processedBuffer.addEvent(currentMessage, samplePos);
				}
			}
		}
		else
		{
			processedBuffer.addEvent(currentMessage, samplePos);
		}
	}
	midiMessages.swapWith(processedBuffer);
}

void MidiProcessor::setTranslationTable(TranslationMidiTable table) {
	translationTable = table;
	updateTranslationMap();
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
	updateTranslationMap();
}

void MidiProcessor::setInputMidi(int index, juce::String midiName)
{
	std::string note = midiName.toStdString();

	std::regex midiNoteRegex(midiNameRegex);
	bool isNoteName = std::regex_match(note, midiNoteRegex);

	translationTable[index].inputMIDI = midiName;
	if (isNoteName) {
		try {
			translationTable[index].inputMIDInumber = nameToNumber.at(note);
		}
		catch (const std::out_of_range& oor) {
			DBG("MidiNota no encontrada " << midiName);
		}
	}
	updateTranslationMap();
}

void MidiProcessor::setInputFantasyName(int index, juce::String inputFantasyName)
{
	std::string note = inputFantasyName.toStdString();

	translationTable[index].inputMIDI = inputFantasyName;

	updateTranslationMap();
}

void MidiProcessor::setOutputFantasyName(int index, juce::String outputFantasyName)
{
	std::string note = outputFantasyName.toStdString();

	translationTable[index].inputMIDI = outputFantasyName;

	updateTranslationMap();
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
		vectorEntry.setProperty("active", item.active, nullptr);
		vectorEntry.setProperty("inputFantasyName", item.inputFantasyName, nullptr);
		vectorEntry.setProperty("outputFantasyName", item.outputFantasyName, nullptr);

		vectorTree.addChild(vectorEntry, -1, nullptr);
	}
	return vectorTree;
}

void MidiProcessor::loadTranslationMapFromValueTree(juce::ValueTree mapTree) {

	std::map<int, int> translationMap;

	for (int i = 0; i < mapTree.getNumChildren(); ++i) {
		auto entry = mapTree.getChild(i);

		// Obtener las propiedades del ValueTree
		int key = entry.getProperty("Key");
		int value = entry.getProperty("Value");

		// Actualizar el mapa
		translationMap[key] = value;

		// Crear la fila y aadirla al vector

	}
	translationMap = translationMap;
}

void MidiProcessor::loadTranslationTableFromValueTree(juce::ValueTree mapTree) {

	TranslationMidiTable translationTableV;
	std::map<int, int> loadedTranslationMap;
	for (int i = 0; i < mapTree.getNumChildren(); ++i) {
		auto vectorEntry = mapTree.getChild(i);
		MidiTranslationRow value = { i + 1, vectorEntry.getProperty("inputMIDInumber"), vectorEntry.getProperty("inputMIDI"), vectorEntry.getProperty("outputMIDI") , 
			vectorEntry.getProperty("outputMIDInumber"), vectorEntry.getProperty("active"), vectorEntry.getProperty("inputFantasyName"), vectorEntry.getProperty("outputFantasyName") };
		translationTableV.push_back(value);
	}

	translationTable = translationTableV;
	updateTranslationMap();
	notify("translationMidiTable", translationTable);
}

TranslationMidiTable MidiProcessor::addTranslationBlock(const MidiTranslationRow& descriptor) {
	auto& table = translationTable;

	MidiTranslationRow row = descriptor;
	row.id = this->nextTranslationMapIndex();

	// Insertar el elemento en la tabla
	table.push_back(row);

	// Actualizar la tabla dentro del objeto
	this->setTranslationTable(table);

	return table; // Retornar la tabla actualizada
}

TranslationMidiTable MidiProcessor::modifyTranslationRow(const MidiTranslationRow& descriptor) {
	auto& table = translationTable;

	MidiTranslationRow row = descriptor;
	int index = findIndex(table, [&descriptor](const MidiTranslationRow& row) {
		return row.id == descriptor.id;
	});;

	if (index != -1) {
		table[index] = descriptor;
		this->setTranslationTable(table);
	}
	else {
		std::cerr << "Error: No se encontró el elemento con id = " << descriptor.id << ".\n";
	}

	return table;
}

TranslationMidiTable MidiProcessor::removeTranslationBlock(int id) {
	auto& table = translationTable;

	// Buscar el elemento con el campo id == id y eliminarlo
	auto it = std::find_if(table.begin(), table.end(), [id](const auto& item) {
		return item.id == id;
		});

	if (it != table.end()) {
		table.erase(it); // Eliminar el elemento encontrado
		this->setTranslationTable(table);
	}

	return table;
}

void MidiProcessor::clearTranslationTable() {
	setTranslationTable({});
}

void MidiProcessor::loadInputMap(std::vector<MapElement> inputMap) {
	translationTable.clear();
	int maxId = this->nextTranslationMapIndex();
	try {
		for (const auto& i : inputMap) {
			translationTable.push_back({
				maxId++,
				i.midiNumber,
				juce::MidiMessage::getMidiNoteName(i.midiNumber, true, true, 3),
				"", 0, true, i.fantasyName, ""
				});
		}
	}
	catch (const std::exception& e) {
		DBG("Error al cargar el input map: " + juce::String(e.what()));
	}
	catch (...) {
		DBG("Error desconocido al cargar el input map.");
	}
	updateTranslationMap();
	notify("translationMidiTable", translationTable);
}

int MidiProcessor::nextTranslationMapIndex() {
	auto& table = translationTable;

	// Encontrar el ID máximo actualmente en la tabla
	int maxId = 0;
	if (!table.empty()) {
		maxId = std::max_element(table.begin(), table.end(), [](const auto& a, const auto& b) {
			return a.id < b.id;
			})->id;
	};
	int id = maxId + 1; 
	DBG(id);
	return id;
}

void MidiProcessor::togglePassThroughEnabled() { passThroughEnabled = !passThroughEnabled; };
void MidiProcessor::toggleTriggerMultiple() { triggerMultiple = !triggerMultiple; }
bool MidiProcessor::isPassThroughEnabled() { return passThroughEnabled; }
bool MidiProcessor::isTriggerMultiple() { return triggerMultiple; }
void MidiProcessor::setPassThroughEnabled(bool enabled) { passThroughEnabled = enabled; }
void MidiProcessor::setTriggerMultiple(bool enabled) { triggerMultiple = enabled; }