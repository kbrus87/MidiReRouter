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
	int id;
	int inputMIDInumber;
	juce::String inputMIDI;
	juce::String outputMIDI;
	int outputMIDInumber;
	juce::String inputFantasyName = "";
	juce::String outputFantasyName = "";
	bool active;

	MidiTranslationRow() = default;

	MidiTranslationRow(int id, int inputMIDInumber, const juce::String& inputMIDI,
		const juce::String& outputMIDI, int outputMIDInumber, bool active,
		const juce::String& inputFantasyName = "", const juce::String& outputFantasyName = "")
		: id(id),
		inputMIDInumber(inputMIDInumber),
		inputMIDI(inputMIDI),
		outputMIDI(outputMIDI),
		outputMIDInumber(outputMIDInumber),
		inputFantasyName(inputFantasyName),
		outputFantasyName(outputFantasyName),
		active(active)
	{
	}

	explicit MidiTranslationRow(const juce::Array<juce::var>& data) {
		if (data.size() != 6) {
			DBG(data.size());
			throw std::invalid_argument("El array debe contener al menos 6 elementos");
		}

		try {
			id = static_cast<int>(data[0]);
			inputMIDInumber = static_cast<int>(data[1]);
			inputMIDI = data[2];
			outputMIDI = data[3];
			outputMIDInumber = static_cast<int>(data[4]);
			active = static_cast<bool>(data[5]);

			// Opcionales si están
			if (data.size() > 6) inputFantasyName = data[6].toString();
			if (data.size() > 7) outputFantasyName = data[7].toString();
		}
		catch (...) {
			throw std::invalid_argument("Error desconocido al convertir los elementos del array.");
		}
	}

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
	using VariantType = std::variant<int, std::string, TranslationMidiTable, juce::String, juce::var>;

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
						rowObject->setProperty("id", row.id);
						rowObject->setProperty("inputMIDInumber", row.inputMIDInumber);
						rowObject->setProperty("inputMIDI", row.inputMIDI);
						rowObject->setProperty("outputMIDI", row.outputMIDI);
						rowObject->setProperty("outputMIDInumber", row.outputMIDInumber);
						rowObject->setProperty("inputFantasyName", row.inputFantasyName);
						rowObject->setProperty("outputFantasyName", row.outputFantasyName);
						rowObject->setProperty("active", row.active);
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

template <typename Container, typename Predicate>
int findIndex(const Container& container, Predicate pred) {
	auto it = std::find_if(container.begin(), container.end(), pred);
	return (it != container.end()) ? std::distance(container.begin(), it) : -1;
}