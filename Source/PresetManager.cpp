/*
  ==============================================================================

	PresetManager.cpp
	Created: 19 Jan 2025 10:47:51pm
	Author:  Bruno Ramari

  ==============================================================================
*/

#include "PresetManager.h"

namespace Service {

	const juce::File PresetManager::defaultDirectory{ juce::File::getSpecialLocation(juce::File::SpecialLocationType::commonDocumentsDirectory)
		.getChildFile(ProjectInfo::companyName).getChildFile(ProjectInfo::projectName)
	};
	const juce::String PresetManager::extension{ "transMap" };

	PresetManager::PresetManager(MidiProcessor& midiProcessor) :midiProcessor(midiProcessor) {
		if (!defaultDirectory.exists()) {
			const auto result = defaultDirectory.createDirectory();
			if (result.failed()) {
				DBG("ERROR creando preset folder");
				DBG(result.getErrorMessage());
				jassertfalse;
			}
		}
	}

	void PresetManager::savePreset(juce::String presetName) {
		if (presetName.isEmpty()) return;

		const auto xml = midiProcessor.translationTableToValueTree().createXml();
		const auto presetFile = defaultDirectory.getChildFile(presetName + "." + extension);

		currentPreset = juce::String(presetName);
		if (!xml->writeTo(presetFile)) {
			DBG("PROBLEMA AL ESCRIBIR " + presetFile.getFullPathName());
			jassertfalse;
		}
		else {
			notify("presetChange", presetName);
		}
	}

	void PresetManager::loadPreset(juce::String presetName) {
		if (presetName.isEmpty()) return;

		const auto presetFile = defaultDirectory.getChildFile(presetName + "." + extension);
		if (!presetFile.existsAsFile()) return;
		DBG("File Exists");
		juce::XmlDocument xmlDocument{ presetFile };
		const auto valueTreeToLoad = juce::ValueTree::fromXml(*xmlDocument.getDocumentElement()); 
		
		midiProcessor.loadTranslationTableFromValueTree(valueTreeToLoad);
		currentPreset = juce::String(presetName);
		notify("presetChange", presetName);
	}

	juce::String PresetManager::getCurrentPreset() {
		return currentPreset;
	}
	void PresetManager::setCurrentPreset(juce::String preset) {
		currentPreset = preset;
		notify("presetChange", preset);
	}
}

