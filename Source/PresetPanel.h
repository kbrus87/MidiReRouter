/*
  ==============================================================================

	PresetBar.h
	Created: 19 Jan 2025 10:07:43pm
	Author:  Bruno Ramari

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PresetManager.h"
#include "Listener.h"

class PresetPanel : public juce::Component, public juce::Button::Listener, public Listener
{
public:
	PresetPanel(Service::PresetManager&, juce::WebBrowserComponent&);
	~PresetPanel();

	void configureButton(juce::Button&,const juce::String&, juce::Colour);
	void resized() override;
	Service::PresetManager getPresetManager() { return presetManager; };

	juce::String presetFunction(juce::String);

	void onEvent(const std::string& identifier, const std::variant<int, std::string, TranslationMidiTable, juce::String, juce::var>& preset);
private:
	Service::PresetManager& presetManager;
	juce::WebBrowserComponent& webView;
	void buttonClicked(juce::Button*) override;
	juce::TextButton saveButton, loadButton;
	std::unique_ptr<juce::FileChooser> fileChooser;
	juce::Label presetLabel;
};