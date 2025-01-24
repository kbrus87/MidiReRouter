/*
  ==============================================================================

    WebView.h
    Created: 22 Jan 2025 11:46:43pm
    Author:  Bruno Ramari

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Listener.h"


class WebView : public juce::WebBrowserComponent, public Listener
{
public:
    void onEvent(const std::string& identifier, const std::variant<int, std::string, TranslationMidiTable, juce::String>& payload);
};