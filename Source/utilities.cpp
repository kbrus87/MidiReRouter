/*
  ==============================================================================

	utilities.cpp
	Created: 23 Apr 2025 6:45:15pm
	Author:  Bruno Ramari

  ==============================================================================
*/

#include "utilities.h"


void Utilities::consoleLog(const juce::String& message, juce::WebBrowserComponent& webView)
{
	// Log the message to the console
	webView.evaluateJavascript("console.log('" + message + "');");
}

void Utilities::openBrowser(const std::string& url)
{
	// Open the URL in the default web browser
#if defined(_WIN32)
	std::string command = "start " + url;
#elif defined(__APPLE__)
	std::string command = "open " + url;
#else // Linux y otros
	std::string command = "xdg-open " + url;
#endif

	system(command.c_str());
}