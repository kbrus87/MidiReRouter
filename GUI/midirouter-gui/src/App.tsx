import React, { useEffect, useState } from 'react';
import './App.css';
import PresetPanel from 'components/PresetPanel/PresetPanel';
import * as juce from "juce";

import Header from 'components/Header/Header';
import MidiTableComponent from 'components/MidiTableComponent/MidiTableComponent';

function App() {
  const [translationTable, setTranslationTable] = useState([])

  useEffect(() => {
    fetch(juce.getBackendResourceAddress("data.json")).then(res => res.text()).then(res => {
      console.log("getBackendResoureAddress", res)
      setTranslationTable(JSON.parse(res).translationTable);
      console.log(JSON.parse(res).translationTable)
    })

    const translationMidiTableEventToken = window.__JUCE__.backend.addEventListener("translationMidiTable", (res) => {
      console.log(res)
    })
    return () => {
      window.__JUCE__.backend.removeEventListener(translationMidiTableEventToken)
    }
  }, [])
  return (
    <div className="App">
      <div className="main-container">
        <Header />
        <div style={{ gridRow: "2/3" }} >
          <PresetPanel />
        </div>
        <div style={{ gridRow: "4/-1" }} >
          <MidiTableComponent translationTable={translationTable}/>
        </div>
      </div>
    </div>
  );
}

export default App;
