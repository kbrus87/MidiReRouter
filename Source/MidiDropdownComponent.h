/*
  ==============================================================================

    MidiDropdownComponent.h
    Created: 13 Jan 2025 8:02:09pm
    Author:  PC

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "types.h"
#include <vector>
#include "TableMidiComponent.h"

class MidiDropdownComponent : public juce::Component
{
public:
    MidiDropdownComponent();

    void setMidiEvents(const std::vector<MidiEventElement>& events);
    juce::String getSelectedEvent();
    MidiEventElement getSelectedMidiNote(juce::String noteName);
    void resized() override;
    void MidiDropdownComponent::addInMidiNote(juce::String midiNoteName, MidiTableComponent& table);
    void MidiDropdownComponent::filterMidiEvents();
    void MidiDropdownComponent::onChange( MidiTableComponent& table);

private:

    std::vector<MidiEventElement> midiEvents;
    juce::ComboBox midiDropdown;
    juce::TextEditor searchTextBox;
	juce::String optiontext;

    std::vector<MidiTranslationRow> defaultEvents;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiDropdownComponent)
};
