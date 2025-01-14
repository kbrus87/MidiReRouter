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

class MidiDropdownComponent : public juce::Component
{
public:
    MidiDropdownComponent(std::vector<MidiEventRow>* midiEventsRouted = nullptr);

    void setMidiEvents(const std::vector<MidiEventElement>& events);
    juce::String getSelectedEvent();

    void resized() override;


private:
    void filterMidiEvents();
	std::vector<MidiEventRow>& midiEventsRoutedRef;
    std::vector<MidiEventElement> midiEvents;
    juce::ComboBox midiDropdown;
    juce::TextEditor searchTextBox;
	juce::String optiontext;

    std::vector<MidiEventRow> defaultEvents;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiDropdownComponent)
};
