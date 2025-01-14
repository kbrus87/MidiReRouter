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
    MidiDropdownComponent();

    void setMidiEvents(const std::vector<MidiEventRow>& events);
    juce::String getSelectedEvent() const;

    void resized() override;


private:
    void filterMidiEvents();
    std::vector<MidiEventRow> midiEvents;
    juce::ComboBox midiDropdown;
    juce::TextEditor searchTextBox;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiDropdownComponent)
};
