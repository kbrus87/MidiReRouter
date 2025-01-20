/*
  ==============================================================================

	PresetBar.cpp
	Created: 19 Jan 2025 10:07:43pm
	Author:  Bruno Ramari

  ==============================================================================
*/

#include "PresetPanel.h"

PresetPanel::PresetPanel(Service::PresetManager& presetManager) :presetManager(presetManager) {

	configureButton(saveButton, "Save Preset", juce::Colour::fromRGB(23, 34, 125));
	configureButton(loadButton, "Load Preset", juce::Colour::fromRGB(23, 34, 125));

	presetLabel.setText(presetManager.getCurrentPreset(), juce::NotificationType::sendNotification);

	addAndMakeVisible(presetLabel);
	addAndMakeVisible(saveButton);
	addAndMakeVisible(loadButton);
}

PresetPanel::~PresetPanel() {
	saveButton.removeListener(this);
	loadButton.removeListener(this);
}

void PresetPanel::configureButton(juce::Button& button, const juce::String& text, juce::Colour color) {
	button.setButtonText(text);
	button.setMouseCursor(juce::MouseCursor::PointingHandCursor);
	button.addListener(this);
	button.setColour(4, color);
	button.repaint();
}

void PresetPanel::resized() {
	const auto container = getLocalBounds().reduced(4);
	auto bounds = container;

	saveButton.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.25f)).reduced(4));
	loadButton.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.25f)).reduced(4));
	presetLabel.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.4f)).reduced(8));
}


void PresetPanel::buttonClicked(juce::Button* button) {
	if (button == &saveButton) {
		fileChooser = std::make_unique<juce::FileChooser>(
			"Name your Preset", Service::PresetManager::defaultDirectory,
			"*." + Service::PresetManager::extension
		);
		fileChooser->launchAsync(juce::FileBrowserComponent::saveMode, [&](const juce::FileChooser& chooser) {
			const auto result = chooser.getResult();
			try {
				presetManager.savePreset(result.getFileNameWithoutExtension());
			}
			catch (int n) {
				DBG("NO SE GUARDO NADA");
			}
			});
	}

	if (button == &loadButton) {
		fileChooser = std::make_unique<juce::FileChooser>(
			"Name your Preset", Service::PresetManager::defaultDirectory,
			"*." + Service::PresetManager::extension
		);
		fileChooser->launchAsync(juce::FileBrowserComponent::openMode, [&](const juce::FileChooser& chooser) {
			const auto result = chooser.getResult();

			presetManager.loadPreset(result.getFileNameWithoutExtension());
			});
	}
}


void PresetPanel::onEvent(const std::string& identifier, const std::variant<int, std::string, TranslationMidiTable, juce::String>& preset) {
	if (identifier == "presetChange")
	{
		DBG("preset PRESET CHANGE");
		//presetLabel.setText(juce::String(preset));
		presetLabel.repaint();
	}
}