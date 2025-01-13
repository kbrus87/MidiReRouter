/*
  ==============================================================================

    MidiProcessor.h
    Created: 7 Jan 2025 4:00:57pm
    Author:  Bruno Ramari

  ==============================================================================
*/

#pragma once

#include <juce_audio_basics/juce_audio_basics.h> // Incluye las clases MidiBuffer y MidiMessage
#include "JuceHeader.h"

class MidiProcessor
{
public:
    void process(juce::MidiBuffer& midiMessages)
    {
        processedBuffer.clear();
        juce::MidiBuffer::Iterator it(midiMessages); // Iterador para recorrer el buffer
        juce::MidiMessage currentMessage;
        int samplePos;

        while (it.getNextEvent(currentMessage, samplePos))
        {

            DBG(currentMessage.getDescription());
            // Si es un evento Note On/Off, modifica el número de nota
            if (currentMessage.isNoteOnOrOff())
            {
                currentMessage.setNoteNumber(50); // Cambia el número de nota a 50
            }

            // Agrega el mensaje al buffer procesado
            processedBuffer.addEvent(currentMessage, samplePos);

        }
        midiMessages.swapWith(processedBuffer);
    }

    juce::MidiBuffer processedBuffer;
};