/*
  ==============================================================================

	Broadcaster.h
	Created: 18 Jan 2025 6:39:36pm
	Author:  Bruno Ramari

  ==============================================================================
*/

#pragma once
#include <vector>
#include <string>
#include <variant>
#include <algorithm>
#include "Listener.h"

class Broadcaster
{
public:
	void addListener(Listener* listener);

	void removeListener(Listener* listener);
	void removeAllListener();

	void notify(const std::string& identifier, const std::variant<int, std::string, TranslationMidiTable, juce::String, juce::var>& payload);
	void notify(const std::string& identifier, const std::vector<MapElement>& map);

private:
	std::vector<Listener*> listeners;
};