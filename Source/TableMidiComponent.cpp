/*
  ==============================================================================

	TableMidiComponent.cpp
	Created: 13 Jan 2025 4:38:25pm
	Author:  Bruno Ramari

  ==============================================================================
*/

#include "TableMidiComponent.h"


MidiTableComponent::MidiTableComponent(TranslationMidiTable& events, std::function<void(int, const juce::String&)> updateMidiEvent, juce::WebBrowserComponent& webView)
	: midiEventList(events), updateMidiEvent(updateMidiEvent), webView(webView) // Inicializacin de la referencia
{
	// Hacer visible la tabla


	addAndMakeVisible(midiTable);
	midiTable.setModel(this);

	// Configurar columnas de la tabla
	midiTable.getHeader().addColumn("Input MIDI", 1, 200, 30);
	midiTable.getHeader().addColumn("Output MIDI", 2, 200, 30);

	midiTable.getHeader().setStretchToFitActive(true);
	setSize(420, 600);
}

MidiTableComponent::~MidiTableComponent()
{
	midiTable.setModel(nullptr); // Desvincular el modelo antes de destruirlo
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
	auto& header = midiTable.getHeader();
	header.setColumnWidth(1, midiTable.getWidth()*0.5f); // 50% del ancho para la columna 1
	header.setColumnWidth(2, midiTable.getWidth() * 0.5f); // 50% del ancho para la columna 2
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

		const float fontSize = juce::jmin(height * 0.6f, 20.0f); // Mximo de 20.0f, ajustable
		g.setFont(fontSize);

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

void MidiTableComponent::onEvent(const std::string& identifier, const std::variant<int, std::string, TranslationMidiTable, juce::String, juce::var>& payload)
{
	if (identifier == "translationMidiTable")
	{
		midiTable.updateContent();
		static const juce::Identifier EVENT_ID("translationMidiTable");
		VariantWrapper payloadWrapper{ payload };
		webView.emitEventIfBrowserIsVisible(EVENT_ID, payloadWrapper.toVar());
	}
}

void MidiTableComponent::onEvent(const std::string& identifier, const std::vector<MapElement>& payload)
{
}

juce::Component* MidiTableComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate)
{
	if (columnId == 2) // Solo procesamos la columna 2
	{
		// Verificar si ya existe un componente
		if (existingComponentToUpdate != nullptr)
		{
			// Intentar hacer un dynamic_cast al tipo esperado
			MidiTextBox* textbox = dynamic_cast<MidiTextBox*>(existingComponentToUpdate);
			if (textbox != nullptr)
			{
				// Actualizar el texto del componente existente
				textbox->setText(midiEventList[rowNumber].outputMIDI);
				return textbox; // Reutilizar el componente existente
			}
			else
			{
				// Si no es del tipo esperado, devolver nullptr (esto puede causar que el TableListBox elimine el componente)
				return nullptr;
			}
		}
		else
		{
			// Crear un nuevo componente si no existe ninguno
			return new MidiTextBox(rowNumber, midiEventList[rowNumber].outputMIDI, updateMidiEvent);
		}
	}

	// Devolver nullptr para columnas que no procesamos
	return nullptr;
}