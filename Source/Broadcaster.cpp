/*
  ==============================================================================

    Broadcaster.cpp
    Created: 18 Jan 2025 6:39:36pm
    Author:  Bruno Ramari

  ==============================================================================
*/

#include "Broadcaster.h"

void Broadcaster::addListener(Listener* listener)
{
    listeners.push_back(listener);
}

void Broadcaster::removeListener(Listener* listener)
{
    listeners.erase(
        std::remove(listeners.begin(), listeners.end(), listener),
        listeners.end()
    );
}

void Broadcaster::removeAllListener()
{
    listeners.clear();
}




void Broadcaster::notify(const std::string& identifier, const std::variant<int, std::string, TranslationMidiTable, juce::String, juce::var>& payload)
{
    for (auto* listener : listeners)
    {
        if (listener)
            listener->onEvent(identifier, payload);
    }
}