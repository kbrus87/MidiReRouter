import React, { useState, useEffect } from 'react';
import * as juce from "juce";

const PresetPanel: React.FC<{ presetName: string; }> = ({ presetName }) => {

    const clickButton = juce.getNativeFunction("presetFunction");
    const handleLoadPreset = async () => {
        try {
            await clickButton("loadButton");

        } catch (error) {
            console.error("Error al cargar preset:", error);
        }
    };

    const handleSavePreset = async () => {
        try {
            await clickButton("saveButton");
        } catch (error) {
            console.error("Error al guardar preset:", error);
        }
    };

    return (
        <div
            style={{
                display: 'flex',
                alignItems: 'center',
                justifyContent: 'space-between',
                height: '2rem', // Aproximadamente 2 rem (puedes ajustar según tu base font-size)
                width: '100%',
                backgroundColor: '#f4f4f4',
                padding: '0 1rem',
                boxShadow: '0 2px 4px rgba(0,0,0,0.1)',
            }}
        >
            <button onClick={handleLoadPreset} style={{ cursor: 'pointer' }}>
                Load
            </button>
            <span>{presetName}</span>
            <button onClick={handleSavePreset} style={{ cursor: 'pointer' }}>
                Save
            </button>
        </div>
    );
};

export default PresetPanel;