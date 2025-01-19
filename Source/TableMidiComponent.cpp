/*
  ==============================================================================

	TableMidiComponent.cpp
	Created: 13 Jan 2025 4:38:25pm
	Author:  Bruno Ramari

  ==============================================================================
*/

#include "TableMidiComponent.h"


MidiTableComponent::MidiTableComponent(TranslationMidiTable& events, std::function<void(int, const juce::String&)> updateMidiEvent)
	: midiEventList(events), updateMidiEvent(updateMidiEvent) // Inicializacin de la referencia
{
	// Hacer visible la tabla

	addAndMakeVisible(midiTable);
	midiTable.setModel(this);

	// Configurar columnas de la tabla
	midiTable.getHeader().addColumn("Income MIDI", 1, 200);
	midiTable.getHeader().addColumn("Output MIDI", 2, 200);


	setSize(600, 600);
}


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
	return midiEventList.size();
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

		if (columnId == 1)
		{
			text = row.inputMIDI;
			g.drawText(text, 2, 0, width - 4, height, juce::Justification::centredLeft);
		}
		else if (columnId == 2)
		{
			text = row.outputMIDI;
			g.drawText(text, 2, 0, width - 4, height, juce::Justification::centredRight);

		}

	}
}


//void MidiTableComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
//{
//	midiTable.updateContent();
//}

void MidiTableComponent::onEvent(const std::string& identifier, const std::variant<int, std::string, TranslationMidiTable>& payload)
{
	if (identifier == "translationMidiTable")
	{
		midiTable.updateContent();
	}
}

juce::Component* MidiTableComponent::refreshComponentForCell(int	rowNumber, int	columnId, bool	isRowSelected, Component* existingComponentToUpdate)
{

	if (columnId == 2)
	{

		if (existingComponentToUpdate != nullptr)
		{
			// Intentar hacer un dynamic_cast a MidiTextBox
			MidiTextBox* textbox = dynamic_cast<MidiTextBox*>(existingComponentToUpdate);
			if (textbox == nullptr)
				return nullptr;

			// Si el componente es válido, actualizar el texto
			textbox->setText(midiEventList[rowNumber].outputMIDI);
			return textbox;
		}
		else
		{
			MidiTextBox* midiTextBox = new MidiTextBox(rowNumber, midiEventList[rowNumber].outputMIDI, updateMidiEvent);
			return midiTextBox;
		}
		//if (existingComponentToUpdate != nullptr)
		//{
		//	// Si el componente ya existe, actualizamos sus propiedades
		//	juce::TextEditor* textbox = dynamic_cast<juce::TextEditor*>(existingComponentToUpdate);
		//	if (textbox == nullptr)
		//		return nullptr;
		//	if (textbox != nullptr)
		//	{
		//		// Cambiar el texto del botón según el número de fila
		//		textbox->setText(midiEventList[rowNumber].outputMIDI);
		//		textbox->setColour(juce::TextButton::buttonColourId, isRowSelected ? juce::Colours::lightblue : juce::Colours::white);
		//		return textbox;
		//	}
		//}
		//else
		//{
		//	// Si no existe un componente, creamos uno nuevo
		//	juce::TextEditor* textbox = new juce::TextEditor("midiOutput" + rowNumber);
		//	textbox->setText(midiEventList[rowNumber].outputMIDI);
		//	juce::String upperCaseText = textbox->getText().toUpperCase();

		//	textbox->setText(upperCaseText);

		//	std::string note = (textbox->getText()).toStdString();

		//	std::regex midiNoteRegex("^(C|D|E|F|G|A|B)(#?)([0-9])$");
		//	bool isNoteName = std::regex_match(note, midiNoteRegex);

		//	DBG(std::to_string(isNoteName) + " << " + note);

		//	if (isNoteName) {
		//		textbox->applyColourToAllText(juce::Colours::green, true);
		//		textbox->repaint();
		//	}
		//	else {
		//		textbox->applyColourToAllText(juce::Colours::red, true);
		//		textbox->repaint();
		//	}

		//	textbox->onTextChange = [this, textbox]()
		//		{
		//			if (textbox != nullptr)
		//			{
		//				juce::String upperCaseText = textbox->getText().toUpperCase();

		//				textbox->setText(upperCaseText);

		//				std::string note = (textbox->getText()).toStdString();

		//				std::regex midiNoteRegex("^(C|D|E|F|G|A|B)(#?)([0-9])$");
		//				bool isNoteName = std::regex_match(note, midiNoteRegex);

		//				DBG(std::to_string(isNoteName) + " << " + note);

		//				if (isNoteName) {
		//					textbox->applyColourToAllText(juce::Colours::green, true);
		//					textbox->repaint();
		//				}
		//				else {
		//					textbox->applyColourToAllText(juce::Colours::red, true);
		//					textbox->repaint();
		//				}
		//			}
		//		};
		//	textbox->onFocusLost = [this, textbox, rowNumber]() {

		//		std::string note = (textbox->getText()).toStdString();

		//		std::regex midiNoteRegex("^(C|D|E|F|G|A|B)(#?)([0-9])$");
		//		bool isNoteName = std::regex_match(note, midiNoteRegex);
		//		//if (!isNoteName) textbox->setText("Invalid MIDI Note");
		//		if (isNoteName) {

		//			updateMidiEvent(rowNumber, textbox->getText());
		//		}
		//		};

		//	return textbox;
		//}
	}
	else
	{
		// Si no estamos en la columna 2, no hacer nada y usar el comportamiento predeterminado
		return nullptr;
	}
}