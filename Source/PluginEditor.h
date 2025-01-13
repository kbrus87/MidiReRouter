/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "types.h"
#include "TableMidiComponent.h"

//==============================================================================
/**
*/
class NewProjectAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    NewProjectAudioProcessorEditor(NewProjectAudioProcessor&);
    ~NewProjectAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NewProjectAudioProcessor& audioProcessor;
    juce::TableListBox midiTable;

    // Datos de la tabla: lista fija de eventos MIDI posibles
    std::vector<MidiEventRow> midiEventList = {
        { "Note On C1", "" },
        { "Note On C#1", "" },
		{ "Note On D1", "" },
		{ "Note On D#1", "" },
		{ "Note On E1", "" },
		{ "Note On F1", "" },
		{ "Note On F#1", "" },
		{ "Note On G1", "" },
		{ "Note On G#1", "" },
		{ "Note On A1", "" },
		{ "Note On A#1", "" },
		{ "Note On B1", "" },
		{ "Note On C2", "" },
		{ "Note On C#2", "" },
		{ "Note On D2", "" },
		{ "Note On D#2", "" },
		{ "Note On E2", "" },
		{ "Note On F2", "" },
		{ "Note On F#2", "" },
		{ "Note On G2", "" },
		{ "Note On G#2", "" },
		{ "Note On A2", "" },
		{ "Note On A#2", "" },
		{ "Note On B2", "" },
		{ "Note On C3", "" },
		{ "Note On C#3", "" },
		{ "Note On D3", "" },
		{ "Note On D#3", "" },
		{ "Note On E3", "" },
		{ "Note On F3", "" },
		{ "Note On F#3", "" },
		{ "Note On G3", "" },
		{ "Note On G#3", "" },
		{ "Note On A3", "" },
		{ "Note On A#3", "" },
		{ "Note On B3", "" },
		{ "Note On C4", "" },
		{ "Note On C#4", "" },
		{ "Note On D4", "" },
		{ "Note On D#4", "" },
		{ "Note On E4", "" },
		{ "Note On F4", "" },
		{ "Note On F#4", "" },
		{ "Note On G4", "" },
		{ "Note On G#4", "" },
		{ "Note On A4", "" },
		{ "Note On A#4", "" },
		{ "Note On B4", "" },
		{ "Note On C5", "" },

    };

    MidiTableComponent midiTableComponent;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessorEditor)
};
