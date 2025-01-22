import React from 'react';
import './App.css';
import PresetPanel from './components/PresetPanel/PresetPanel';

import Header from './components/Header/Header';

function App() {
  return (
    <div className="App">
      <div className="main-container">
        <Header />
        <div style={{gridRow:"1/2"}}>
          <PresetPanel />
        </div>
      </div>
    </div>
  );
}

export default App;
