import React, { useState } from 'react';
import * as juce from "juce";

const PresetPanel: React.FC = () => {
    const [presetName, setPresetName] = useState<string>('No Preset');

    const clickButton = juce.getNativeFunction("presetFunction");
    // Función para cargar el preset (se invoca la función nativa "loadPreset")
    const handleLoadPreset = async () => {
        try {
            // Llamada a la función nativa; ajusta el nombre de la función y los argumentos según tu configuración
            const result = await clickButton("loadButton");
            console.log(result)
            // Suponemos que la función retorna el nombre del preset cargado
            setPresetName(result);
        } catch (error) {
            console.error("Error al cargar preset:", error);
        }
    };

    // Función para guardar el preset (se invoca la función nativa "savePreset")
    const handleSavePreset = async () => {
        try {
            const result = await clickButton("saveButton");
            console.log(result)
            // Suponemos que la función retorna el nombre del preset cargado
            setPresetName(result);
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