/*
  ==============================================================================

    types.h
    Created: 13 Jan 2025 4:20:28pm
    Author:  Bruno Ramari

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>  // Para usar juce::String

struct MidiEventRow
{
    int inputMIDInumber;
    juce::String inputMIDI;
    juce::String outputMIDI;
    int outputMIDInumber;
};

struct MidiEventElement
{
    juce::String midiName;
    int midiNumber;
};