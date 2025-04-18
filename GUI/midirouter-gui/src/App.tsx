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

  useEffect(() => {
    fetch(juce.getBackendResourceAddress("data.json")).then(res => res.text()).then(res => {
      setTranslationTable(JSON.parse(res).translationTable);
      setPresetName(JSON.parse(res).preset);
      console.log(res)
    })


    const translationMidiTableEventToken = window.__JUCE__.backend.addEventListener("translationMidiTable", (res) => {
      setTranslationTable(res)
      console.log("translationMidiTable", res)
    })
    const presetChangeToken = window.__JUCE__.backend.addEventListener("presetChange", (res) => {
      setPresetName(res)
      console.log("presetChange",res)
    })
    return () => {
      window.__JUCE__.backend.removeEventListener(presetChangeToken)
      window.__JUCE__.backend.removeEventListener(translationMidiTableEventToken)
    }
  }, [])

  return (
    <div className="App">
      <div className="main-container">
        <div style={{ zIndex: 10000, gridRow: "1/2", position: "sticky", background: "#1F1F23", top: 0, minHeight: "fit-content" }} >
          <Header />
          <PresetPanel presetName={presetName} />
          <div className="buttons-actions">
            <div className="buttons-actions button clear" onClick={() => juce.getNativeFunction("clearTranslationTable")()}>Clear</div>
            <div className="buttons-actions button add" onClick={() => juce.getNativeFunction("addTranslationBlock")()}>
              <Add className="add-icon" />
              <span className="addhelp">Add a Midi Route</span>
            </div>

          </div>
        </div>
        <div style={{ gridRow: "4/-1" }} >
          <MidiTableComponent translationTable={translationTable} />
        </div>
        <div className="inputtag">Midi In</div>
        <div className="outtag">Midi Out</div>
      </div>
    </div>
  );
}

export default App;
