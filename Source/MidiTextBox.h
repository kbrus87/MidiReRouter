/*
  ==============================================================================

	MidiTextBox.h
	Created: 19 Jan 2025 2:26:30pm
	Author:  Bruno Ramari

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <regex>

class MidiTextBox : public juce::TextEditor
{
public:
	MidiTextBox(int numberRow, const juce::String& text, std::function<void(int, const juce::String&)> update);

private:
	int numberRow;
	std::function<void(int, const juce::String&)> update;
	void validateAndUpdate();
};
