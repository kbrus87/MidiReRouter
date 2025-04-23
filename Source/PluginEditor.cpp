/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include <optional>
#include <unordered_map> // Para el mapa
#include <utility>
#include "PluginProcessor.h"
#include "PluginEditor.h"

#define CSS
#define JS
#define MEDIA

#include <MyBinaryData.h>


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
			{ { "woff2" },  "font/woff2"               },
			{ { "otf"   },  "font/otf"                 },
			{ { "ttf"   },  "font/ttf"                 }
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
		.withNativeFunction(juce::Identifier{ "openBrowser" }, [this](const juce::Array<juce::var>& url, juce::WebBrowserComponent::NativeFunctionCompletion) {
			Utilities::openBrowser(url[0].toString().toStdString());
			})
		.withNativeFunction(juce::Identifier{ "modifyTranslationBlock" }, [this](const juce::Array<juce::var>& row, juce::WebBrowserComponent::NativeFunctionCompletion completion) {

			auto parsed(juce::JSON::parse(row[0].toString()));
			auto dynamicObject = parsed.getDynamicObject();

			juce::String outputMidi = dynamicObject->getProperty("outputMIDI");
			juce::String inputMidi = dynamicObject->getProperty("inputMIDI");

			auto iteratorInputMidiNumber = nameToNumber.find(inputMidi.toStdString());
			auto iteratorOutputMidiNumber = nameToNumber.find(outputMidi.toStdString());

			int inputMidiNumber = -1;
			int outputMidiNumber = -1;

			bool isActive = dynamicObject->getProperty("active") ? true : false;

			if (iteratorInputMidiNumber != nameToNumber.end()) {
				inputMidiNumber = iteratorInputMidiNumber->second;  
			}
			if (iteratorOutputMidiNumber != nameToNumber.end()) {
				outputMidiNumber = iteratorOutputMidiNumber->second;  
			}

			if (iteratorOutputMidiNumber == nameToNumber.end()) {
				outputMidiNumber = inputMidiNumber;
			}
			if (iteratorInputMidiNumber == nameToNumber.end()) {
				isActive = false;
			}

			MidiTranslationRow translationRow({
					dynamicObject->getProperty("id"),                    // id
					inputMidiNumber,
					inputMidi,            // inputMIDI
					outputMidi,           // outputMIDI
					outputMidiNumber,     // outputMIDInumber
					isActive,                // active
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

	//addAndMakeVisible(midiTableComponent);
	//addAndMakeVisible(midiDropdownComponent);
	//addAndMakeVisible(presetPanel);

	auto opts = juce::WebBrowserComponent::Options{}
	.withBackend(juce::WebBrowserComponent::Options::Backend::webview2);

	bool webview2Disponible = juce::WebBrowserComponent::areOptionsSupported(opts);

	//if (juce::WebBrowserComponent::areOptionsSupported(options)) {
	    Utilities::consoleLog("WebView2 is supported", webView);
	    Utilities::consoleLog(webview2Disponible ? "true" : "false", webView);

		addAndMakeVisible(webView);
		webView.goToURL(juce::WebBrowserComponent::getResourceProviderRoot());
		// webView.goToURL(LOCAL_DEV); // only dev
	/*}
	else {
		addAndMakeVisible(fallBack);
	}*/

	DBG("LOCAL_PROD value: " + juce::String(LOCAL_DEV));
	DBG("Registering ResourceProvider for origin: " + juce::URL{ LOCAL_DEV }.getOrigin());
	juce::String initialUrl = LOCAL_DEV + juce::String("index.html"); // Construye la URL completa
	DBG("Calling goToURL with: " + initialUrl);

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

	fallBack.setBounds(getLocalBounds());
	webView.setBounds(getLocalBounds());//.removeFromRight(midiTableComponent.getWidth()));
}

auto MidiRouterProcessorEditor::getResource(const juce::String& url) -> std::optional<juce::WebBrowserComponent::Resource>
{
	DBG("==============================================================");
	DBG(url);
	// return html i nbytes
	
	// Construir la ruta a la carpeta de recursos
	
	const auto resourceToRetrieve = url == "/" ? "index.html" : url.fromFirstOccurrenceOf("/", false, false);

	DBG("Resource to retrieve: " + resourceToRetrieve + ' ' + url);

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


	juce::String resourcePath = url.startsWith("/") ? url.substring(1) : url;
	if (resourcePath.isEmpty() || resourcePath == "/")
	{
		resourcePath = "index.html"; // Asume index.html para la raíz
	}
	juce::String filenameOnly = juce::File::createFileWithoutCheckingPath(resourcePath).getFileName();
	if (filenameOnly.isEmpty()) { // Podría pasar si urlPath era solo "/" o similar
		filenameOnly = "index.html";
	}

	juce::String resourceIdentifier = filenameOnly;
	resourceIdentifier = resourceIdentifier.replaceCharacter('.', '_');
	resourceIdentifier = resourceIdentifier.removeCharacters("-");
	resourceIdentifier = resourceIdentifier.substring(resourceIdentifier.lastIndexOf("/")+1).trim();

	DBG("Attempting to find BinaryData identifier: " + resourceIdentifier);
	int dataSize = 0;

	const char* resourceData = frontend::getNamedResource(resourceIdentifier.toRawUTF8(), dataSize);
	if (resourceData != nullptr && dataSize > 0)
	{
		DBG("Found BinaryData resource: " + resourceIdentifier + ", Size: " + juce::String(dataSize));

		std::vector<std::byte> dataVec((std::byte*)resourceData, (std::byte*)resourceData + dataSize);

		juce::String extension = getExtension(resourcePath);
		const char* mimeType = getMimeForExtension(extension);
		DBG("Serving resource " + resourcePath + " with MIME type: " + mimeType);

		return juce::WebBrowserComponent::Resource{ std::move(dataVec), juce::String(mimeType) };
	}
	else
	{
		DBG("BinaryData resource not found: " + resourceIdentifier + " (Original path: " + resourcePath + ")");
		return std::nullopt;
	}

	return std::nullopt;
}

