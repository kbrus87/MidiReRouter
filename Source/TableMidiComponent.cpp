/*
  ==============================================================================

	TableMidiComponent.cpp
	Created: 13 Jan 2025 4:38:25pm
	Author:  Bruno Ramari

  ==============================================================================
*/

#include "TableMidiComponent.h"

MidiTableComponent::MidiTableComponent(TranslationMidiTable& events)
	: midiEventList(events) // Inicializacin de la referencia
{
	// Hacer visible la tabla

	addAndMakeVisible(midiTable);
	midiTable.setModel(this);

	// Configurar columnas de la tabla
	midiTable.getHeader().addColumn("Income MIDI", 1, 200);
	midiTable.getHeader().addColumn("Output MIDI", 2, 200);


	setSize(400, 300);
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
	// Si estamos en la columna 2, crear o actualizar un componente (por ejemplo, un botón)
	if (columnId == 2)
	{
		if (existingComponentToUpdate != nullptr)
		{
			// Si el componente ya existe, actualizamos sus propiedades
			auto* button = dynamic_cast<juce::TextButton*>(existingComponentToUpdate);
			if (button != nullptr)
			{
				// Cambiar el texto del botón según el número de fila
				button->setButtonText("Fila " + juce::String(rowNumber));
				button->setColour(juce::TextButton::buttonColourId, isRowSelected ? juce::Colours::lightblue : juce::Colours::white);
			}
		}
		else
		{
			// Si no existe un componente, creamos uno nuevo
			auto* button = new juce::TextButton("Fila " + juce::String(rowNumber));
			button->setClickingTogglesState(true);
			button->setColour(juce::TextButton::buttonColourId, isRowSelected ? juce::Colours::lightblue : juce::Colours::white);
			button->setBounds(0, 0, 100, 30); // Tamaño del botón

			// Devolver el nuevo componente para la columna 2
			return button;
		}
	}
	else
	{
		// Si no estamos en la columna 2, no hacer nada y usar el comportamiento predeterminado
		return nullptr; // Esto hace que la celda use el comportamiento por defecto (solo texto)
	}
}