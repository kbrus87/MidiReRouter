/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <juce_gui_extra/juce_gui_extra.h>
#include "PluginProcessor.h"
#include "types.h"
#include "TableMidiComponent.h"
#include "MidiDropdownComponent.h"
#include "constants.h"
#include "PresetPanel.h"
#include "WebView.h"

//==============================================================================
/**
*/
class MidiRouterProcessorEditor : public juce::AudioProcessorEditor
{
public:
	MidiRouterProcessorEditor(MidiRouterProcessor&);
	~MidiRouterProcessorEditor() override;

	//==============================================================================
	void paint(juce::Graphics&) override;
	void resized() override;

private:
	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	MidiRouterProcessor& audioProcessor;
	MidiProcessor& midiProcessor;

	juce::WebBrowserComponent webView;

	std::optional<juce::WebBrowserComponent::Resource> getResource(const juce::String& url);
			

	juce::TableListBox midiTable;

	MidiDropdownComponent midiDropdownComponent;

	PresetPanel presetPanel;
	MidiTableComponent midiTableComponent;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiRouterProcessorEditor)
};
