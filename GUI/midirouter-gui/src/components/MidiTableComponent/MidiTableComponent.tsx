import React from "react";
import * as juce from "juce";


function MidiTableComponent({ translationTable }: { translationTable: MidiTableEventRow[] }) {
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
                    const isInputNull = t.inputMIDI?.trim() === "" || !t.inputMIDI;
                    const isOutputNull = t.outputMIDI?.trim() === "" || !t.outputMIDI;

                    return <>
                        <div key={t.id} style={{ display: "flex", alignItems: "center", justifyContent: "end" }}>
                            <div onClick={() => {
                                removeBlock(t.id)
                            }} className="delete-button">
                                X
                            </div>
                        </div>
                        <div key={t.inputMIDInumber + "_" + t.outputMIDInumber} className="midiTableRow">
                            <div className={`midiTableCell Input  ${isInputNull ? "MidiNull" : "MidiNote"}`}>{isInputNull ? ".." : t.inputMIDI}</div>
                            <MidiLink row={t} modifyBlock={modifyBlock} isInputNull={isInputNull} isOutputNull={isOutputNull} isActive={t.active} />
                            <div className={`midiTableCell Output  ${isOutputNull ? "MidiNull" : "MidiNote"}`}>{isOutputNull ? ".." : t.outputMIDI}</div>
                        </div>
                    </>
                })
            }
        </div>
    </div>
}


function MidiLink({ row, isInputNull, isOutputNull, isActive, modifyBlock }: { row: MidiTableEventRow, isInputNull: boolean; isOutputNull: boolean; isActive: boolean; modifyBlock: (row: MidiTableEventRow) => void }) {

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
    return <div onClick={toggleActivate} className={`midiTableCell Link ${isInputNull ? isOutputNull ? "FullNull" : "LeftNull" : isOutputNull ? "RightNull" : "LeftNote"} `}>
        <div className={`circle ${isInputNull ? "red" : "active-red"}`}></div>
        <div className={`circle ${isOutputNull ? "blue" : "active-blue"}`}></div>
    </div>
}

export default MidiTableComponent