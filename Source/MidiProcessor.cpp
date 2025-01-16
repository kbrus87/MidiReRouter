/*
  ==============================================================================

    MidiProcessor.cpp
    Created: 16 Jan 2025 8:23:02pm
    Author:  Bruno Ramari

  ==============================================================================
*/

#include "MidiProcessor.h"

void MidiProcessor::process(juce::MidiBuffer& midiMessages)
{
    processedBuffer.clear();
    juce::MidiBuffer::Iterator it(midiMessages); // Iterador para recorrer el buffer
    juce::MidiMessage currentMessage;
    int samplePos;

    while (it.getNextEvent(currentMessage, samplePos))
    {
        if (currentMessage.isController())
        {
            DBG("es Controlador: "; currentMessage.isController());
            DBG("controllerNumber: "; currentMessage.getControllerNumber());
            DBG(currentMessage.getControllerNumber());
            DBG("value: "; currentMessage.getControllerValue());
            DBG(currentMessage.getControllerValue());
        }

        // Si es un evento Note On/Off, modifica el número de nota


        if (currentMessage.isNoteOnOrOff())
        {
            DBG("eS NOTA");
            DBG("es Nota: "; currentMessage.isNoteOnOrOff());
            DBG("numero nota: "; currentMessage.getNoteNumber());

            currentMessage.setNoteNumber(50); // Cambia el número de nota a 50
        }

        // Agrega el mensaje al buffer procesado
        processedBuffer.addEvent(currentMessage, samplePos);

    }
    midiMessages.swapWith(processedBuffer);
}
