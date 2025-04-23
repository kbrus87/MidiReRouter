/*
  ==============================================================================

    utilities.h
    Created: 23 Apr 2025 6:46:37pm
    Author:  Bruno Ramari

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
class Utilities
{
public:
    void static consoleLog(const juce::String& message, juce::WebBrowserComponent& webView);
    void static openBrowser(const std::string& url);
};
