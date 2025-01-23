import React from 'react';
import './App.css';
import PresetPanel from './components/PresetPanel/PresetPanel';
import * as juce from "juce";

import Header from './components/Header/Header';

function App() {

  console.log(window.__JUCE__.backend);
  return (
    <div className="App">
      <div className="main-container">
        <Header />
        <div style={{gridRow:"2/3"}} >
          <PresetPanel />
        </div>
      </div>
    </div>
  );
}

export default App;
