/*
  ==============================================================================

	PresetManager.h
	Created: 19 Jan 2025 10:47:51pm
	Author:  Bruno Ramari

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "MidiProcessor.h"
#include "Broadcaster.h"

namespace Service
{
	class PresetManager : public Broadcaster 
	{
	public:
		const static juce::File defaultDirectory;
		const static juce::String extension;

		PresetManager(MidiProcessor& midiProcessor);

		void savePreset(juce::String presetName);
		void loadPreset(juce::File presetFile);
		void loadInputMap(juce::File drummap);

		void setOutputMapList(juce::File drummap);

		juce::String getCurrentPreset();
		void setCurrentPreset(juce::String preset);

		std::vector<MapElement> parseDrumMap(const juce::File& file);

	private:
		MidiProcessor& midiProcessor;
		juce::String currentPreset;
		juce::String currentInputMap;
		std::vector<MapElement> outputMapList;
	};
};