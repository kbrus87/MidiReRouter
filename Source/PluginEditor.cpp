/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
namespace {

	static const char* getMimeForExtension(const juce::String& extension)
	{
		static const std::unordered_map<juce::String, const char*> mimeMap =
		{
			{ { "htm"   },  "text/html"                },
			{ { "html"  },  "text/html"                },
			{ { "txt"   },  "text/plain"               },
			{ { "jpg"   },  "image/jpeg"               },
			{ { "jpeg"  },  "image/jpeg"               },
			{ { "svg"   },  "image/svg+xml"            },
			{ { "ico"   },  "image/vnd.microsoft.icon" },
			{ { "json"  },  "application/json"         },
			{ { "png"   },  "image/png"                },
			{ { "css"   },  "text/css"                 },
			{ { "map"   },  "application/json"         },
			{ { "js"    },  "text/javascript"          },
			{ { "woff2" },  "font/woff2"               }
		};

		if (const auto it = mimeMap.find(extension.toLowerCase()); it != mimeMap.end())
			return it->second;

		jassertfalse;
		return "";
	}

	static juce::String getExtension(juce::String filename)
	{
		return filename.fromLastOccurrenceOf(".", false, false);
	}

	static auto streamToVector(juce::InputStream& stream)
	{
		std::vector<std::byte> result((size_t)stream.getTotalLength());
		stream.setPosition(0);
		[[maybe_unused]] const auto bytesRead = stream.read(result.data(), result.size());
		jassert(bytesRead == (juce::ssize_t)result.size());
		return result;
	}
}
const juce::String LOCAL_DEV = "http://localhost:3000/";
//==============================================================================
MidiRouterProcessorEditor::MidiRouterProcessorEditor(MidiRouterProcessor& p)
	: AudioProcessorEditor(&p),
	audioProcessor(p),
	midiProcessor(p.getMidiProcessor()),

	webView(juce::WebBrowserComponent::Options{}
		.withBackend(juce::WebBrowserComponent::Options::Backend::webview2)
		.withWinWebView2Options(juce::WebBrowserComponent::Options::WinWebView2{}.withUserDataFolder(juce::File::getSpecialLocation(juce::File::tempDirectory)))
		.withResourceProvider([this](const auto& url) {
			return getResource(url);
			}, juce::URL{ LOCAL_DEV }.getOrigin())
		.withNativeIntegrationEnabled()
		.withNativeFunction(juce::Identifier{ "addTranslationBlock" }, [this](const juce::Array<juce::var>&, juce::WebBrowserComponent::NativeFunctionCompletion)
			{
				midiProcessor.addTranslationBlock();
			})
		.withNativeFunction(juce::Identifier{ "removeTranslationBlock" }, [this](const juce::Array<juce::var>& index, juce::WebBrowserComponent::NativeFunctionCompletion)
			{
				midiProcessor.removeTranslationBlock(index[0]);
			})
		.withNativeFunction(juce::Identifier{ "clearTranslationTable" }, [this](const juce::Array<juce::var>&, juce::WebBrowserComponent::NativeFunctionCompletion)
			{
				midiProcessor.clearTranslationTable();
			})
		.withNativeFunction(juce::Identifier{ "presetFunction" }, [this](const juce::Array<juce::var>& buttonName, juce::WebBrowserComponent::NativeFunctionCompletion completion) {
			presetPanel.presetFunction(buttonName[0]);
			})
		.withNativeFunction(juce::Identifier{ "modifyTranslationBlock" }, [this](const juce::Array<juce::var>& row, juce::WebBrowserComponent::NativeFunctionCompletion completion) {

			auto parsed(juce::JSON::parse(row[0].toString()));
			auto dynamicObject = parsed.getDynamicObject();

			juce::String outputMidi = dynamicObject->getProperty("outputMIDI");
			juce::String inputMidi = dynamicObject->getProperty("inputMIDI");

			auto iteratorOutputMidiNumber = nameToNumber.find(outputMidi.toStdString());
			auto iteratorInputMidiNumber = nameToNumber.find(inputMidi.toStdString());

			int outputMidiNumber = -1;
			int inputMidiNumber = -1;

			if (iteratorOutputMidiNumber != nameToNumber.end()) {
				outputMidiNumber = iteratorOutputMidiNumber->second;  
			}
			if (iteratorInputMidiNumber != nameToNumber.end()) {
				inputMidiNumber = iteratorInputMidiNumber->second;  
			}

			
			MidiTranslationRow translationRow({
					dynamicObject->getProperty("id"),                    // id
					inputMidiNumber,
					inputMidi,            // inputMIDI
					outputMidi,           // outputMIDI
					outputMidiNumber,     // outputMIDInumber
					dynamicObject->getProperty("active"),                // active
					dynamicObject->getProperty("inputFantasyName"),      // inputFantasyName
					dynamicObject->getProperty("outputFantasyName")     // outputFantasyName
			});

			midiProcessor.modifyTranslationRow(translationRow);

			})
	), presetPanel(p.getPresetManager(), webView), midiTableComponent(
		midiProcessor.translationTable,
		std::bind(&MidiProcessor::setOutputMidi, &midiProcessor, std::placeholders::_1, std::placeholders::_2),
		webView
	)
{
	//midiProcessor.addChangeListener(&midiTableComponent);
	midiProcessor.addListener(&midiTableComponent);
	p.getPresetManager().addListener(&presetPanel);

	midiTableComponent.setMidiEvents(midiProcessor.translationTable);

	midiDropdownComponent.setMidiEvents(constMidiEventList);
	midiDropdownComponent.onChange(midiProcessor);

	addAndMakeVisible(midiTableComponent);
	addAndMakeVisible(midiDropdownComponent);
	addAndMakeVisible(presetPanel);

	// midiProcessor.addListener(&webView);
	addAndMakeVisible(webView);
	webView.goToURL(LOCAL_DEV);
	//webView.goToURL(webView.getResourceProviderRoot());

	setResizable(true, true);
	setSize(midiTableComponent.getWidth() * 2.2, 500);
}

MidiRouterProcessorEditor::~MidiRouterProcessorEditor()
{
	//midiProcessor.removeAllChangeListeners();
	midiProcessor.removeAllListener();
	audioProcessor.getPresetManager().removeAllListener();
}

//==============================================================================
void MidiRouterProcessorEditor::paint(juce::Graphics& g)
{
	// Fondo del componente
	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

	// Calcular tamaño de fuente proporcional al tamaño del editor
	float fontSize = getHeight() * 0.05f; // Por ejemplo, 5% de la altura
	g.setColour(juce::Colours::white);
	g.setFont(juce::FontOptions(fontSize));

	// Áreas dinámicas basadas en proporciones del tamaño del editor
	juce::Rectangle<int> area(getWidth() * 0.4, getHeight() * 0.1, getWidth() * 0.8, fontSize);
	juce::Rectangle<int> area2(getWidth() * 0.4, getHeight() * 0.1 + area.getHeight(), getWidth() * 0.8, fontSize);

	// Dibujar texto con las áreas redimensionadas
	g.drawFittedText("Midi Router", area, juce::Justification::centred, 1);

	g.setFont(juce::FontOptions(fontSize * 0.8f));
	g.drawFittedText("by BJR", area2, juce::Justification::centred, 1);

}

void MidiRouterProcessorEditor::resized()
{
	// This is generally where you'll want to lay out the positions of any
	// subcomponents in your editor..

	midiDropdownComponent.setBounds(getLocalBounds().reduced(8).removeFromLeft(proportionOfWidth(0.024f)).getX(),
		getLocalBounds().removeFromTop(proportionOfHeight(0.1f)).getBottom(), proportionOfWidth(0.29f), proportionOfHeight(0.1f)
	);
	//midiTableComponent.setBounds(getLocalBounds().removeFromBottom(proportionOfHeight(0.78f)).reduced(8));
	midiTableComponent.setBounds(
		getLocalBounds()
		.removeFromBottom(proportionOfHeight(0.78f)) // Define la altura según proportionOfHeight
		.removeFromLeft(getWidth() / 2) // Ajusta para ocupar solo la mitad izquierda
		.reduced(8) // Añade márgenes
	);

	presetPanel.setBounds(getLocalBounds().removeFromTop(proportionOfHeight(0.07f)));


	webView.setBounds(getLocalBounds());//.removeFromRight(midiTableComponent.getWidth()));
}

auto MidiRouterProcessorEditor::getResource(const juce::String& url) -> std::optional<juce::WebBrowserComponent::Resource>
{
	DBG("==============================================================");
	DBG(url);
	// return html i nbytes
	// Obtener la ruta del directorio del ejecutable
	static const auto executableDir = juce::File::getSpecialLocation(juce::File::currentExecutableFile)
		.getParentDirectory();

	// Construir la ruta a la carpeta de recursos
	static const auto resourceFileRoot = juce::File(R"(C:/Users/Bruno Ramari/Documents/JuceProjects/Midi effect/NewProject/GUI/midirouter-gui/build)");
	DBG(resourceFileRoot.getFullPathName());
	const auto resourceToRetrieve = url == "/" ? "index.html" : url.fromFirstOccurrenceOf("/", false, false);

	if (resourceToRetrieve == "data.json") {
		juce::DynamicObject::Ptr data{ new juce::DynamicObject{} };
		// Obtener la tabla de traducción del midiProcessor
		auto translationTable = midiProcessor.getTranslationTable();
		juce::Array<juce::var> tableArray;
		// Convertir cada elemento del std::vector a juce::var
		for (const auto& entry : translationTable) {
			// Crear un objeto DynamicObject para cada entrada
			juce::DynamicObject::Ptr entryObject{ new juce::DynamicObject{} };

			// Asignar las propiedades del tuple al objeto
			entryObject->setProperty("id", entry.id); // int
			entryObject->setProperty("inputMIDI", entry.inputMIDI); // int
			entryObject->setProperty("inputMIDInumber", entry.inputMIDInumber); // juce::String
			entryObject->setProperty("outputMIDI", entry.outputMIDI); // juce::String
			entryObject->setProperty("outputMIDInumber", entry.outputMIDInumber); // int
			entryObject->setProperty("active", entry.active); // int
			entryObject->setProperty("inputFantasyName", entry.inputFantasyName); // int
			entryObject->setProperty("outputFantasyName", entry.outputFantasyName); // int

			// Añadir el objeto al array
			tableArray.add(juce::var(entryObject.get()));
		}

		// Asignar el array al objeto data
		data->setProperty("translationTable", tableArray);
		data->setProperty("preset", presetPanel.getPresetManager().getCurrentPreset());

		const auto string = juce::JSON::toString(data.get());
		juce::MemoryInputStream stream{ string.getCharPointer(), string.getNumBytesAsUTF8(), false };
		return juce::WebBrowserComponent::Resource{ streamToVector(stream), juce::String("application/json") };
	}

	const auto resource = resourceFileRoot.getChildFile(resourceToRetrieve).createInputStream();

	if (resource) {
		const auto extension = resourceToRetrieve.fromLastOccurrenceOf(".", false, false);
		return juce::WebBrowserComponent::Resource{ streamToVector(*resource), getMimeForExtension(extension) };
	}

	return std::nullopt;
}

