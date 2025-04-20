/*
  ==============================================================================

    FallBack.cpp
    Created: 19 Apr 2025 11:53:32pm
    Author:  PC

  ==============================================================================
*/

#include "FallBack.h"
FallBack::FallBack()
    : webViewLink("Install WebView2",
        juce::URL("https://developer.microsoft.com/en-us/microsoft-edge/webview2/"))
    {
        // Título
        titleLabel.setText("WebView2 Runtime Required", juce::dontSendNotification);
        titleLabel.setFont(juce::Font(18.0f, juce::Font::bold));
        titleLabel.setJustificationType(juce::Justification::centred);
        addAndMakeVisible(titleLabel);

        // Caja de texto con mensaje
        messageBox.setMultiLine(true);
        messageBox.setReadOnly(true);
        messageBox.setText(
            "This VST requires the WebView2 Runtime to display its interface.\n\n"
            "Please install the WebView2 Runtime to continue using this plugin properly.\n"
            "Click the link below to get more information or download the installer.\n\n"
            "Or copy and paste this URL into your browser:\n"
            "https://developer.microsoft.com/en-us/microsoft-edge/webview2/"
        );
        messageBox.setColour(juce::TextEditor::backgroundColourId, juce::Colours::transparentBlack);
        messageBox.setColour(juce::TextEditor::outlineColourId, juce::Colours::transparentBlack);
        messageBox.setColour(juce::TextEditor::textColourId, juce::Colours::white);
        messageBox.setFont(juce::Font(15.0f));
        addAndMakeVisible(messageBox);

        // Botón con link
        // webViewLink.setFont(juce::Font(15.0f, juce::Font::underlined));
        webViewLink.setColour(juce::HyperlinkButton::textColourId, juce::Colours::cyan);
        addAndMakeVisible(webViewLink);
    }

	void FallBack::paint(juce::Graphics& g)
	{

	}

	void FallBack::resized()
	{
        auto area = getLocalBounds().reduced(20);

        titleLabel.setBounds(area.removeFromTop(30));
        area.removeFromTop(10);

        messageBox.setBounds(area.removeFromTop(140));
        area.removeFromTop(10);

        webViewLink.setBounds(area.removeFromTop(24));
	}
