import React from 'react';
import * as juce from "juce";

import { ReactComponent as Save } from "assets/svg/save2.svg";
import { ReactComponent as Load } from "assets/svg/load.svg";

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
            className="presetPanelBar"
        >
            <div className="presetPanelBar presetName">
                <span>{presetName}</span>
            </div>
            <div className="presetPanelBar buttonsPanel">
                <div onClick={handleSavePreset} className="presetPanelBar buttonsPanel save" >
                    <Save className="save-icon" />
                </div>
                <div onClick={handleLoadPreset} className="presetPanelBar buttonsPanel load" style={{ cursor: 'pointer' }}>
                    <Load className="load-icon" /> Load Preset
                </div>
            </div>
        </div>
    );
};

export default PresetPanel;