/*
  ==============================================================================

    WebView.cpp
    Created: 22 Jan 2025 11:46:43pm
    Author:  Bruno Ramari

  ==============================================================================
*/

#include "WebView.h"

void WebView::onEvent(const std::string& identifier, const std::variant<int, std::string, TranslationMidiTable, juce::String>& payload){
    if (identifier == "translationMidiTable")
    {
        static const juce::Identifier EVENT_ID("poronga");
        this->emitEventIfBrowserIsVisible(EVENT_ID, "mira esta poronga");
    }
    return;
}