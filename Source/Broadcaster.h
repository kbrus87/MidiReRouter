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

	void notify(const std::string& identifier, const std::variant<int, std::string, TranslationMidiTable>& payload);

private:
	std::vector<Listener*> listeners;
};