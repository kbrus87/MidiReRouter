/*
  ==============================================================================

    types.h
    Created: 13 Jan 2025 4:20:28pm
    Author:  Bruno Ramari

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>  // Para usar juce::String

struct MidiTranslationRow
{
    int inputMIDInumber;
    juce::String inputMIDI;
    juce::String outputMIDI;
    int outputMIDInumber;
    bool active;
};

using TranslationMidiTable = std::vector<MidiTranslationRow>;

struct MidiEventElement
{
    juce::String midiName;
    int midiNumber;
};

struct MidiEventElementR
{
    int midiNumber;
    juce::String midiName;
};

class VariantWrapper
{
public:
    using VariantType = std::variant<int, std::string, TranslationMidiTable,juce::String, juce::var>;

    explicit VariantWrapper(VariantType v) : value(std::move(v)) {}

    const VariantType& getValue() const { return value; }

    juce::var toVar() const
    {
        return std::visit(
            [](auto&& arg) -> juce::var {
                using T = std::decay_t<decltype(arg)>;

                if constexpr (std::is_same_v<T, int>) {
                    return juce::var(arg);
                }
                else if constexpr (std::is_same_v<T, std::string>) {
                    return juce::var(juce::String(arg));
                }
                else if constexpr (std::is_same_v<T, juce::String>) {
                    return juce::var(arg); // juce::String es compatible directamente con juce::var
                }
                else if constexpr (std::is_same_v<T, TranslationMidiTable>) {
                    juce::Array<juce::var> tableArray;
                    for (const auto& row : arg) {
                        juce::DynamicObject::Ptr rowObject{ new juce::DynamicObject{} };
                        rowObject->setProperty("inputMIDInumber", row.inputMIDInumber);
                        rowObject->setProperty("inputMIDI", row.inputMIDI);
                        rowObject->setProperty("outputMIDI", row.outputMIDI);
                        rowObject->setProperty("outputMIDInumber", row.outputMIDInumber);
                        tableArray.add(juce::var(rowObject.get()));
                    }
                    return tableArray;
                }
                else if constexpr (std::is_same_v<T, juce::var>) {
                    return arg; // Si ya es un juce::var, simplemente lo retornas
                }
                else {
                    static_assert(always_false<T>::value, "Unsupported type in variant");
                }
            },
            value
        );
    }

    // Helper para static_assert
    template<typename>
    struct always_false : std::false_type {};

private:
    VariantType value;
};