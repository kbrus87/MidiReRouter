/*
  ==============================================================================

    MidiDropdownComponent.cpp
    Created: 13 Jan 2025 8:02:28pm
    Author:  PC

  ==============================================================================
*/

#include "MidiDropdownComponent.h"
#include "juce_gui_basics/juce_gui_basics.h" 

MidiDropdownComponent::MidiDropdownComponent()
{
    addAndMakeVisible(searchTextBox);
    searchTextBox.setTextToShowWhenEmpty("Search MIDI Event...", juce::Colour());
    searchTextBox.onTextChange = [this] { filterMidiEvents(); };  // Filtra los eventos cuando el texto cambie

	

    addAndMakeVisible(midiDropdown);
}

void MidiDropdownComponent::setMidiEvents(const std::vector<MidiEventRow>& events)
{
    midiEvents = events;  // Guardar los eventos MIDI
    filterMidiEvents();   // Filtrar eventos al inicio (vacío)
}


//void MidiDropdownComponent::setMidiEvents(const std::vector<MidiEventRow>& events)
//{
//    midiDropdown.clear();  // Limpia cualquier opción anterior
//
//    for (size_t i = 0; i < events.size(); ++i)
//    {
//        midiDropdown.addItem(events[i].inputMIDI, static_cast<int>(i + 1));
//    }
//}

juce::String MidiDropdownComponent::getSelectedEvent() const
{
    return midiDropdown.getText();
}

void MidiDropdownComponent::resized()
{
    // Establecer las posiciones del TextEditor y ComboBox (midiDropdown)
    auto bounds = getLocalBounds();
    searchTextBox.setBounds(bounds.removeFromTop(30));  // Coloca el cuadro de búsqueda arriba
    midiDropdown.setBounds(bounds);  // El ComboBox ocupa el resto del espacio

}

void MidiDropdownComponent::filterMidiEvents()
{
    juce::String searchText = searchTextBox.getText();  // Obtener el texto de búsqueda
    midiDropdown.clear();  // Limpiar las opciones anteriores

    // Filtrar los eventos MIDI
    for (size_t i = 0; i < midiEvents.size(); ++i)
    {
        // Compara el texto ingresado con la descripción del evento MIDI
        if (midiEvents[i].inputMIDI.containsIgnoreCase(searchText))
        {
            midiDropdown.addItem(midiEvents[i].inputMIDI, static_cast<int>(i + 1));  // Agregar el evento filtrado
        }
    }
}