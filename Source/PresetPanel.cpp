/*
  ==============================================================================

	PresetBar.cpp
	Created: 19 Jan 2025 10:07:43pm
	Author:  Bruno Ramari

  ==============================================================================
*/

#include "PresetPanel.h"

PresetPanel::PresetPanel(Service::PresetManager& presetManager, juce::WebBrowserComponent& webView) :presetManager(presetManager), webView(webView) {

	configureButton(saveButton, "Save Preset", juce::Colour::fromRGB(23, 34, 125));
	configureButton(loadButton, "Load Preset", juce::Colour::fromRGB(23, 34, 125));
	configureButton(loadInputMap, "Load Input DrumMap", juce::Colour::fromRGB(23, 34, 125));
	configureButton(setOutputMapList, "set OutputMap List", juce::Colour::fromRGB(23, 34, 125));

	presetLabel.setText(presetManager.getCurrentPreset(), juce::NotificationType::dontSendNotification);

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
	loadInputMap.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.25f)).reduced(4));
	presetLabel.setBounds(bounds.removeFromLeft(container.proportionOfWidth(0.4f)).reduced(4));
	presetLabel.setFont(juce::Font(juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.5f));
}

juce::String PresetPanel::presetFunction(juce::String button) {
	if (button == "saveButton") {
		saveButton.triggerClick();
		return presetManager.getCurrentPreset();
	}

	if (button == "loadButton") {
		loadButton.triggerClick();
		return presetManager.getCurrentPreset();
	}

	if (button == "loadInputMap") {
		loadInputMap.triggerClick();
		return presetManager.getCurrentPreset();
	}
	
	if (button == "setOutputMapList") {
		setOutputMapList.triggerClick();
		return presetManager.getCurrentPreset();
	}
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

			presetManager.loadPreset(result);
			}
		);
	}

	if (button == &loadInputMap) {
		fileChooser = std::make_unique<juce::FileChooser>(
			"Drum Map", Service::PresetManager::defaultDirectory,
			"*.txt"
		);
		fileChooser->launchAsync(juce::FileBrowserComponent::openMode, [&](const juce::FileChooser& chooser) {
			const auto result = chooser.getResult();
			presetManager.loadInputMap(result);
			}
		);
	}
	
	if (button == &setOutputMapList) {
		fileChooser = std::make_unique<juce::FileChooser>(
			"Drum Map", Service::PresetManager::defaultDirectory,
			"*.txt"
		);
		fileChooser->launchAsync(juce::FileBrowserComponent::openMode, [&](const juce::FileChooser& chooser) {
			const auto result = chooser.getResult();
			presetManager.setOutputMapList(result);
			}
		);
	}
}


void PresetPanel::onEvent(const std::string& identifier, const std::variant<int, std::string, TranslationMidiTable, juce::String, juce::var>& preset) {
	if (identifier == "presetChange")
	{

		juce::String presetText;

		// Verificar cada tipo contenido en std::variant y convertirlo a juce::String
		if (std::holds_alternative<int>(preset)) {
			// Convertir un int a juce::String
			presetText = juce::String(std::get<int>(preset));
		}
		else if (std::holds_alternative<std::string>(preset)) {
			// Convertir un std::string a juce::String
			presetText = juce::String(std::get<std::string>(preset));
		}
		else if (std::holds_alternative<juce::String>(preset)) {
			// Directamente usar juce::String
			presetText = std::get<juce::String>(preset);
		}

		presetLabel.setText(presetText, juce::NotificationType::sendNotification);
		presetLabel.repaint();


		// to comunicate to webView
		static const juce::Identifier EVENT_ID("presetChange");
		VariantWrapper payloadWrapper{ preset };
		webView.emitEventIfBrowserIsVisible(EVENT_ID, payloadWrapper.toVar());
	}

}

void PresetPanel::onEvent(const std::string& identifier, const std::vector<MapElement>& payload)
{
	// Lo que quieras hacer con el vector
	if (identifier == "outputMapList")
	{
		DBG("Recibido vector de MapElement con ID: " + juce::String(identifier));
		// to comunicate to webView
		static const juce::Identifier EVENT_ID("outputMapList");

		
		VariantArrayWrapper payloadWrapper{ payload };
		webView.emitEventIfBrowserIsVisible(EVENT_ID, payloadWrapper.toVar());

	}
}
