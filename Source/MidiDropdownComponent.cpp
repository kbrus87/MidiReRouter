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
#include "TableMidiComponent.h"
#include <vector>



MidiDropdownComponent::MidiDropdownComponent()
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
    
    return optiontext;
}


MidiEventElement MidiDropdownComponent::getSelectedMidiNote(juce::String param_noteName)
{
    MidiEventElement midiNote = {};

    for (MidiEventElement el : midiEvents)
    {
        if (el.midiName == param_noteName)
        {
            midiNote = el;
            break;
        };
    }

    return midiNote;
}

void MidiDropdownComponent::addInMidiNote(juce::String midiNoteName, MidiTableComponent& table)
{
    if (midiNoteName.isEmpty())return;

    MidiEventElement selectedInMidiNote = MidiDropdownComponent::getSelectedMidiNote(midiNoteName);
    std::vector<MidiEventRow> transMidiList = table.getMidiEvents();

    MidiEventRow newMidiEventRow = { selectedInMidiNote.midiNumber, selectedInMidiNote.midiName, "", 0};

    transMidiList.push_back(newMidiEventRow);
    table.setMidiEvents(transMidiList);
    return ;
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

void MidiDropdownComponent::onChange( MidiTableComponent& param_table) {
    midiDropdown.onChange = [this, &param_table] {MidiDropdownComponent::addInMidiNote(midiDropdown.getText(), param_table); };
}
