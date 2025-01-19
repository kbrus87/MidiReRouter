/*
  ==============================================================================

	MidiTextBox.cpp
	Created: 19 Jan 2025 2:26:30pm
	Author:  Bruno Ramari

  ==============================================================================
*/

#include "MidiTextBox.h"

MidiTextBox::MidiTextBox(int numberRow, const juce::String& text, std::function<void(int, const juce::String&)> update) : update(update), numberRow(numberRow) {
	onTextChange = [this, numberRow, update]() {
		juce::String upperCaseText = this->getText().toUpperCase();
		this->setText(upperCaseText);
		this->validateAndUpdate();
		};
}

void MidiTextBox::validateAndUpdate()
{
	std::string note = getText().toStdString();
	std::regex midiNoteRegex("^(C|D|E|F|G|A|B)(#?)([0-9])$");
	bool isNoteName = std::regex_match(note, midiNoteRegex);

	// Cambiar el color del texto basado en la validación
	if (isNoteName) {
		applyColourToAllText(juce::Colours::green, true);
	}
	else {
		applyColourToAllText(juce::Colours::red, true);
	}

	update(numberRow, getText());
	repaint();
}