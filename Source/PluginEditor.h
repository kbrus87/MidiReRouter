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
    NewProjectAudioProcessorEditor (NewProjectAudioProcessor&);
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
    };

    MidiTableComponent midiTableComponent;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessorEditor)
};
