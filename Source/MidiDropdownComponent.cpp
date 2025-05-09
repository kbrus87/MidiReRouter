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
    filterMidiEvents();   // Filtrar eventos al inicio (vac�o)
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

void MidiDropdownComponent::addInMidiNote(juce::String midiNoteName, MidiProcessor& midiProcessor)
{
    if (midiNoteName.isEmpty())return;

    MidiEventElement selectedInMidiNote = MidiDropdownComponent::getSelectedMidiNote(midiNoteName);
    //TranslationMidiTable transMidiList = midiProcessor.getTranslationTable();

    //MidiTranslationRow newMidiEventRow = { selectedInMidiNote.midiNumber, selectedInMidiNote.midiName, "", 0};
    //transMidiList.push_back(newMidiEventRow);
    //midiProcessor.setTranslationTable(transMidiList);
    
    midiProcessor.addTranslationBlock({0, selectedInMidiNote.midiNumber, selectedInMidiNote.midiName, "", 0, true });
    midiDropdown.setSelectedId(0);
    return ;
}

void MidiDropdownComponent::resized()
{
    // Establecer las posiciones del TextEditor y ComboBox (midiDropdown)
    auto bounds = getLocalBounds();
    midiDropdown.setBounds(bounds.removeFromLeft(bounds.getWidth()*0.8f));  // El ComboBox ocupa el resto del espacio
    searchTextBox.setBounds(bounds.removeFromLeft(midiDropdown.getWidth()));  // Coloca el cuadro de b�squeda arriba
    
}

void MidiDropdownComponent::filterMidiEvents()
{
    juce::String searchText = searchTextBox.getText();  // Obtener el texto de b�squeda
    midiDropdown.clear();  // Limpiar las opciones anteriores

    // Filtrar los eventos MIDI
    for (size_t i = 0; i < midiEvents.size(); ++i)
    {
        // Compara el texto ingresado con la descripci�n del evento MIDI
        if (midiEvents[i].midiName.containsIgnoreCase(searchText))
        {
            midiDropdown.addItem(midiEvents[i].midiName, midiEvents[i].midiNumber);  // Agregar el evento filtrado
        }
    }
}

void MidiDropdownComponent::onChange( MidiProcessor &param_midiProcessor) {
    midiDropdown.onChange = [this, &param_midiProcessor] {MidiDropdownComponent::addInMidiNote(midiDropdown.getText(), param_midiProcessor); };
}
