import React, { useState, Fragment } from "react";
import * as juce from "juce";
import { drummapElement, numberToName } from "types";

const regexp = new RegExp("^(C|D|E|F|G|A|B)(#?)-?([0-9])$");

function MidiTableComponent({ translationTable, outputMapList }: { translationTable: MidiTableEventRow[], outputMapList: drummapElement[] | [] }) {
    const removeBlock = (id: number) => {
        const nativeRemoveBlock = juce.getNativeFunction("removeTranslationBlock")
        nativeRemoveBlock(id)
    }

    const modifyBlock = (row: MidiTableEventRow) => {
        const nativeRemoveBlock = juce.getNativeFunction("modifyTranslationBlock")
        nativeRemoveBlock(JSON.stringify(row))
    }


    return <div className="midiTable">
        <div className="midiTableHeader">
            <div className="midiTableHeaderColumnInput"></div>
            <div className="midiTableHeaderColumnOutput"></div>
        </div>
        <div className="midiTableRows">
            {
                translationTable.map((t) => {
                    const isInputValid = t.inputMIDI?.trim() === "" || !t.inputMIDI || !regexp.test(t.inputMIDI);
                    const isOutputValid = t.outputMIDI?.trim() === "" || !t.outputMIDI || !regexp.test(t.outputMIDI);

                    return <Fragment key={t.id}>
                        <div key={t.id} style={{ display: "flex", alignItems: "center", justifyContent: "end" }}>
                            <div onClick={() => {
                                removeBlock(t.id)
                            }} className="delete-button">
                                X
                            </div>
                        </div>
                        <div key={t.inputMIDInumber + "_" + t.outputMIDInumber} className="midiTableRow">
                            <MidiFantasyNameInput name="inputFantasyName" row={t} modifyBlock={modifyBlock} className={"InputFantasyName"} />
                            <MidiTextInput name="inputMIDI" row={t} modifyBlock={modifyBlock} isValid={isInputValid} className={"Input"} />
                            <MidiLink row={t} modifyBlock={modifyBlock} isInputValid={isInputValid} isOutputValid={isOutputValid} isActive={t.active} />
                            <MidiTextInput name="outputMIDI" row={t} modifyBlock={modifyBlock} isValid={isOutputValid} className={"Output"} />
                            {outputMapList.length === 0 ?

                                <MidiFantasyNameInput name="outputFantasyName" row={t} modifyBlock={modifyBlock} className={"OutputFantasyName"} /> :
                                <MidiFantasySelect row={t} modifyBlock={modifyBlock} outputMapList={outputMapList} />

                            }
                        </div>
                    </Fragment>
                })
            }
        </div>
    </div>
}


function MidiLink({ row, isInputValid, isOutputValid, isActive, modifyBlock }: { row: MidiTableEventRow, isInputValid: boolean; isOutputValid: boolean; isActive: boolean; modifyBlock: (row: MidiTableEventRow) => void }) {

    const toggleActivate = () => {
        row['active'] = !isActive;
        modifyBlock(row);
    }

    if (!isActive) {
        return <div onClick={toggleActivate} className={`midiTableCell Link FullNull`}>
            <div className={`circle red`}></div>
            <div className={`circle blue`}></div>
        </div>
    }
    return <div onClick={toggleActivate} className={`midiTableCell Link ${isInputValid ? isOutputValid ? "FullNull" : "LeftNull" : isOutputValid ? "RightNull" : "LeftNote"} `}>
        <div className={`circle ${isInputValid ? "red" : "active-red"}`}></div>
        <div className={`circle ${isOutputValid ? "blue" : "active-blue"}`}></div>
    </div>
}

function MidiTextInput({ name, row, isValid, modifyBlock, className }: { name: string; row: MidiTableEventRow, isValid: boolean; modifyBlock: (row: MidiTableEventRow) => void, className: string }) {

    const [value, setValue] = useState(row[name as keyof MidiTableEventRow] as string)
    const [validNote, setValidNote] = useState(regexp.test(value));

    const handleMIDIInputChange = (newValue: string) => {
        setValue(newValue)
        setValidNote(regexp.test(newValue))
    }

    const handleBlur = () => {
        const nRow: MidiTableEventRow = { ...row };
        const key = name as keyof MidiTableEventRow;
        (nRow[key] as string) = value;
        modifyBlock(nRow);
    }

    return <div style={{ position: "relative" }} className={`midiTableCell  ${className} ${isValid ? "MidiNull" : "MidiNote"}`}>
        <input
            type="text"
            value={value}
            onChange={(e) => handleMIDIInputChange(e.target.value)}
            onBlur={() => handleBlur()}
            style={{
                position: "absolute",
                top: 0,
                left: 0,
                width: "100%",
                height: "100%",
                backgroundColor: "transparent",
                border: "none",
                color: validNote ? "inherit" : "red",
                textAlign: "inherit",
                // pointerEvents: isInteractive ? "auto" : "none", // Permite alternar interacción según 'isInteractive'
            }} />
    </div>
}

function MidiFantasyNameInput({ name, row, modifyBlock, className }: { name: string; row: MidiTableEventRow, modifyBlock: (row: MidiTableEventRow) => void, className: string }) {
    const [value, setValue] = useState(row[name as keyof MidiTableEventRow] as string)

    const handleBlur = () => {
        const nRow: MidiTableEventRow = { ...row };
        const key = name as keyof MidiTableEventRow;
        (nRow[key] as string) = value;
        modifyBlock(nRow);
    }

    const handleChange = (newValue: string) => {
        setValue(newValue)
    }

    return <div className={className}><input name={name} value={value} onChange={(e) => handleChange(e.target.value)} onBlur={handleBlur} /></div>
}

function MidiFantasySelect({ row, modifyBlock, outputMapList }: { row: MidiTableEventRow, modifyBlock: (row: MidiTableEventRow) => void, outputMapList: drummapElement[] }) {

    const setValidOptionValue = (midiNumberSet: number) => {
        return outputMapList.findIndex(item => item.midiNumber === midiNumberSet) !== -1 ? midiNumberSet : 0
    }

    const handleOutputDrumItemSelection = (e: { target: { value: any; }; }) => {
        const value = Number(e.target.value);
        const nRow: MidiTableEventRow = { ...row };
        nRow.outputMIDInumber = value;
        const outputFantasyName = outputMapList.find((item) => item.midiNumber === value)?.fantasyName;
        nRow.outputFantasyName = outputFantasyName || "";
        nRow.outputMIDI = numberToName.get(value) || "";

        modifyBlock(nRow);
    }

    return <select onChange={handleOutputDrumItemSelection} className="OutputFantasySelect" style={{ color: setValidOptionValue(row.outputMIDInumber) === 0 ? "#A0A1A8" : "#e1e1e1" }} value={setValidOptionValue(row.outputMIDInumber)}>
        {
            outputMapList.map((outputMap) => {
                return <option key={outputMap.midiNumber} value={outputMap.midiNumber}>{outputMap.fantasyName}</option>
            })
        }
        <option key="null" value={0} selected> -- Seleccionar --</option>
    </select>
}
export default MidiTableComponent