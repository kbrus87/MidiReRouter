/*
  ==============================================================================

    FallBack.h
    Created: 19 Apr 2025 11:53:32pm
    Author:  PC

  ==============================================================================
*/

#pragma once


#include <JuceHeader.h>


class FallBack : public juce::Component
{
public:
    FallBack();
    ~FallBack() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    juce::Label titleLabel;
    juce::TextEditor messageBox;
    juce::HyperlinkButton webViewLink;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FallBack)
};