import React, { useEffect, useState } from 'react';
import './App.css';
import PresetPanel from 'components/PresetPanel/PresetPanel';
import * as juce from "juce";

import Header from 'components/Header/Header';
import MidiTableComponent from 'components/MidiTableComponent/MidiTableComponent';

import { ReactComponent as Add } from "assets/svg/add.svg";

function App() {
  const [translationTable, setTranslationTable] = useState([])
  const [presetName, setPresetName] = useState<string>('No Preset');
  const [outputMapList, setOutputMapList] = useState<Array<{ midiNumber: number, fantasyName: string }>>([]);

  const [isPassThroughEnabled, setPassThroughEnabled] = useState(false);
  const [isTriggerMultiple, setTriggerMultiple] = useState(false);

  const clickButton = juce.getNativeFunction("presetFunction");
  const openBrowser = juce.getNativeFunction("openBrowser");

  const togglePassThroughEnabled = () => {
    const ctogglePassThrough = juce.getNativeFunction("togglePassThroughEnabled");
    ctogglePassThrough();
    setPassThroughEnabled(!isPassThroughEnabled)
  };
  
  const toggleTriggerMultiple = () => {
    const ctoggleTriggerMultiple = juce.getNativeFunction("toggleTriggerMultiple")
    ctoggleTriggerMultiple();
    setTriggerMultiple(!isTriggerMultiple)
  };

  useEffect(() => {
    fetch(juce.getBackendResourceAddress("data.json")).then(res => res.text()).then(res => {
      const parsedRes = JSON.parse(res);
      setTranslationTable(parsedRes.translationTable);
      setPresetName(parsedRes.preset);
      setPassThroughEnabled(parsedRes.passThroughEnabled);
      setTriggerMultiple(parsedRes.triggerMultiple);
    })

    const translationMidiTableEventToken = window.__JUCE__.backend.addEventListener("translationMidiTable", (res) => {
      setTranslationTable(res)
    })
    const presetChangeToken = window.__JUCE__.backend.addEventListener("presetChange", (res) => {
      setPresetName(res)
      setOutputMapList([])
    })

    const outputListChangeToken = window.__JUCE__.backend.addEventListener("outputMapList", (res) => {
      setOutputMapList(res)
    })
    return () => {
      window.__JUCE__.backend.removeEventListener(presetChangeToken)
      window.__JUCE__.backend.removeEventListener(translationMidiTableEventToken)
      window.__JUCE__.backend.removeEventListener(outputListChangeToken)
    }
  }, [])

  const handleLoadInputDrumMap = async () => {
    try {
      await clickButton("loadInputMap");

    } catch (error) {
      console.error("Error al input Drump Map:", error);
    }
  };

  const handleSetOutputMapList = async () => {
    try {
      await clickButton("setOutputMapList");

    } catch (error) {
      console.error("Error al input Drump Map:", error);
    }
  };

  const handleClear = async () => {
    try {
      juce.getNativeFunction("clearTranslationTable")()
      setOutputMapList([])

    } catch (error) {
      console.error("Error al input Drump Map:", error);
    }
  };

  return (
    <div className="App">
      <div className="main-container">
        <div style={{ zIndex: 10000, gridRow: "1/2", position: "sticky", background: "#1F1F23", top: 0, minHeight: "fit-content" }} >
          <Header openBrowser={openBrowser} />
          <PresetPanel presetName={presetName} />
          <div className="buttons-actions">
            <div className="buttons-actions button clear" onClick={handleClear}>Clear</div>
            <div className="buttons-actions button clear" onClick={() => handleLoadInputDrumMap()}>Load Input Drumkit</div>
            <div className="buttons-actions button add" onClick={() => juce.getNativeFunction("addTranslationBlock")()}>
              <Add className="add-icon" />
              <span className="addhelp">Add a Midi Route</span>
            </div>
            <div className="buttons-actions button clear" onClick={() => handleSetOutputMapList()}>Load Output Drumkit</div>
            <div style={{backgroundColor:isPassThroughEnabled ? 'rgb(39, 107, 33)' : 'rgba(112, 112, 112, 0.87)'}} className={`buttons-actions button clear`} onClick={() => togglePassThroughEnabled()}>Pass Through</div>
            {/* <div style={{backgroundColor:isTriggerMultiple ? 'rgb(39, 107, 33)' : 'rgba(112, 112, 112, 0.87)'}} className={`buttons-actions button clear`} onClick={() => toggleTriggerMultiple()}>Multi</div>  NOT YET SUPPORTED */}
          </div>
        </div>
        <div style={{ gridRow: "4/-1" }} >
          <MidiTableComponent translationTable={translationTable} outputMapList={outputMapList} />
        </div>
        <div className="inputtag">Midi In</div>
        <div className="outtag">Midi Out</div>
      </div>
    </div>
  );
}

export default App;
