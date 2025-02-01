import React, { useEffect, useState } from 'react';
import './App.css';
import PresetPanel from 'components/PresetPanel/PresetPanel';
import * as juce from "juce";

import Header from 'components/Header/Header';
import MidiTableComponent from 'components/MidiTableComponent/MidiTableComponent';

function App() {
  const [translationTable, setTranslationTable] = useState([])
  const [presetName, setPresetName] = useState<string>('No Preset');

  useEffect(() => {
    fetch(juce.getBackendResourceAddress("data.json")).then(res => res.text()).then(res => {
      setTranslationTable(JSON.parse(res).translationTable);
      setPresetName(JSON.parse(res).preset);
    })

    const translationMidiTableEventToken = window.__JUCE__.backend.addEventListener("translationMidiTable", (res) => {
      setTranslationTable(res)
    })
    const presetChangeToken = window.__JUCE__.backend.addEventListener("presetChange", (res) => {
      setPresetName(res)
    })
    return () => {
      window.__JUCE__.backend.removeEventListener(presetChangeToken)
      window.__JUCE__.backend.removeEventListener(translationMidiTableEventToken)
    }
  }, [])
  
  return (
    <div className="App">
      <div className="main-container">
        <div style={{ gridRow: "1/2", position: "sticky", background: "#1F1F23", top: 0, minHeight: "fit-content" }} >
          <Header />
          <PresetPanel presetName={presetName} />
        </div>
        <div style={{ gridRow: "4/-1" }} >
          <MidiTableComponent translationTable={translationTable} />
        </div>
      </div>
    </div>
  );
}

export default App;
