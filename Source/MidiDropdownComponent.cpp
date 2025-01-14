/*
  ==============================================================================

    MidiDropdownComponent.cpp
    Created: 13 Jan 2025 8:02:28pm
    Author:  PC

  ==============================================================================
*/

#include "MidiDropdownComponent.h"
#include "juce_gui_basics/juce_gui_basics.h" 
#include "PluginEditor.h"

MidiDropdownComponent::MidiDropdownComponent(std::vector<MidiEventRow>* midiEventsRouted) : midiEventsRoutedRef(midiEventsRouted ? *midiEventsRouted : defaultEvents)
{
    addAndMakeVisible(searchTextBox);
    searchTextBox.setTextToShowWhenEmpty("Search MIDI Event...", juce::Colour());
    searchTextBox.onTextChange = [this] { filterMidiEvents(); };  // Filtra los eventos cuando el texto cambie

    addAndMakeVisible(midiDropdown);
}

void MidiDropdownComponent::setMidiEvents(const std::vector<MidiEventElement>& events)
{
    midiEvents = events;  // Guardar los eventos MIDI
    filterMidiEvents();   // Filtrar eventos al inicio (vacío)
}

juce::String MidiDropdownComponent::getSelectedEvent()
{
    optiontext = midiDropdown.getText();  // Obtener el texto seleccionado
    int selectedId = midiDropdown.getSelectedId();

    // Verificar si hay un elemento seleccionado
    if (selectedId > 0)
    {
        // Inicializar la estructura con los valores adecuados
        MidiEventRow newrow = {selectedId,optiontext, "",0};

        midiEventsRoutedRef.push_back(newrow);  // Agregar el evento a la lista de eventos MIDI
    }
    else
    {
        DBG("No valid MIDI event selected.");
    }
    DBG("Text: " + optiontext + " Id: " + juce::String(selectedId));
    return optiontext;
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
        if (midiEvents[i].midiName.containsIgnoreCase(searchText))
        {
            midiDropdown.addItem(midiEvents[i].midiName, midiEvents[i].midiNumber);  // Agregar el evento filtrado
        }
    }
}