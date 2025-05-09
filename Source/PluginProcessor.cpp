/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MidiRouterProcessor::MidiRouterProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	), parameters(*this, nullptr), presetManager(midiProcessor)
#endif
{


}

MidiRouterProcessor::~MidiRouterProcessor()
{
}

//==============================================================================
const juce::String MidiRouterProcessor::getName() const
{
	return JucePlugin_Name;
}

bool MidiRouterProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
	return true;
#else
	return false;
#endif
}

bool MidiRouterProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
	return true;
#else
	return false;
#endif
}

bool MidiRouterProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
	return true;
#else
	return false;
#endif
}

double MidiRouterProcessor::getTailLengthSeconds() const
{
	return 0.0;
}

int MidiRouterProcessor::getNumPrograms()
{
	return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
	// so this should be at least 1, even if you're not really implementing programs.
}

int MidiRouterProcessor::getCurrentProgram()
{
	return 0;
}

void MidiRouterProcessor::setCurrentProgram(int index)
{
}

const juce::String MidiRouterProcessor::getProgramName(int index)
{
	return {};
}

void MidiRouterProcessor::changeProgramName(int index, const juce::String& newName)
{
}

//==============================================================================
void MidiRouterProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	// Use this method as the place to do any pre-playback
	// initialisation that you need..
}

void MidiRouterProcessor::releaseResources()
{
	// When playback stops, you can use this as an opportunity to free up any
	// spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MidiRouterProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
	juce::ignoreUnused(layouts);
	return true;
#else
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	// Some plugin hosts, such as certain GarageBand versions, will only
	// load plugins that support stereo bus layouts.
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
		return false;

	// This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
		return false;
#endif

	return true;
#endif
}
#endif

void MidiRouterProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	buffer.clear();
	midiProcessor.process(midiMessages);
}

//==============================================================================
bool MidiRouterProcessor::hasEditor() const
{
	return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MidiRouterProcessor::createEditor()
{
	return new MidiRouterProcessorEditor(*this);
}

//==============================================================================
void MidiRouterProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.

	juce::ValueTree state("MidiRouterState");
	
	state.addChild(midiProcessor.translationMapToValueTree(), -1, nullptr);
	state.addChild(midiProcessor.translationTableToValueTree(), -1, nullptr);

	juce::ValueTree presetName("presetName");
	presetName.setProperty("presetName", presetManager.getCurrentPreset(), nullptr);

	state.addChild(presetName, -1, nullptr);

	juce::ValueTree settings("Settings");
	settings.setProperty("passThroughEnabled", midiProcessor.isPassThroughEnabled(), nullptr);
	settings.setProperty("triggerMultiple", midiProcessor.isTriggerMultiple(), nullptr);

	state.addChild(settings, -1, nullptr);
	// Convertir el ValueTree a XML y luego a un MemoryBlock
	if (auto xmlState = state.createXml()) {
		copyXmlToBinary(*xmlState, destData);
	}
}

void MidiRouterProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory block,
	// whose contents will have been created by the getStateInformation() call.

	juce::ValueTree state;
	if (auto xmlState = getXmlFromBinary(data, sizeInBytes)) {
		state = juce::ValueTree::fromXml(*xmlState);
	}

	if (state.isValid()) {
		// Restaurar el mapa desde el ValueTree
		juce::ValueTree mapTree = state.getChildWithName("TranslationMap");
		midiProcessor.loadTranslationMapFromValueTree(mapTree);
		// Restaurar el vector desde el ValueTree
		juce::ValueTree vectorTree = state.getChildWithName("TranslationTable");
		midiProcessor.loadTranslationTableFromValueTree(vectorTree);

		juce::ValueTree preset = state.getChildWithName("presetName");
		presetManager.setCurrentPreset(preset.getProperty("presetName"));

		juce::ValueTree settings = state.getChildWithName("Settings");
		if (settings.isValid()) {
			midiProcessor.setPassThroughEnabled(settings.getProperty("passThroughEnabled"));
			midiProcessor.setTriggerMultiple(settings.getProperty("triggerMultiple"));
		}
		else {
			midiProcessor.setPassThroughEnabled(false);
			midiProcessor.setTriggerMultiple(false);
		}
	}
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new MidiRouterProcessor();
}

MidiProcessor& MidiRouterProcessor::getMidiProcessor() {
	return midiProcessor;
};

Service::PresetManager& MidiRouterProcessor::getPresetManager() {
	return presetManager;
}