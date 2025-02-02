import React from "react";
import * as juce from "juce";
import Delsvg from "../../assets/svg/delete.svg";

function MidiTableComponent({ translationTable }: { translationTable: MidiTableEventRow[] }) {
    const removeBlock = (id: number) => {
        const nativeRemoveBlock = juce.getNativeFunction("removeTranslationBlock")
        nativeRemoveBlock(id)
    }
    return <div className="midiTable">
        <div className="midiTableHeader">
            <div className="midiTableHeaderColumnInput"></div>
            <div className="midiTableHeaderColumnOutput"></div>
        </div>
        <div className="midiTableRows">
            {
                translationTable.map((t, i) => {

                    const isInputNull = t.inputMIDI?.trim() === "" || !t.inputMIDI;
                    const isOutputNull = t.outputMIDI?.trim() === "" || !t.outputMIDI;

                    return <>
                        <div style={{display:"flex", alignItems:"center", justifyContent:"end"}}>
                            <div onClick={() => removeBlock(i)} className="delete-button"><Delsvg/></div>
                        </div>
                        <div key={t.inputMIDInumber + "_" + t.outputMIDInumber} className="midiTableRow">
                            <div className={`midiTableCell Input  ${isInputNull ? "MidiNull" : "MidiNote"}`}>{isInputNull ? ".." : t.inputMIDI}</div>
                            <MidiLink isInputNull={isInputNull} isOutputNull={isOutputNull} isActive={t.active} />
                            <div className={`midiTableCell Output  ${isOutputNull ? "MidiNull" : "MidiNote"}`}>{isOutputNull ? ".." : t.outputMIDI}</div>
                        </div>
                    </>
                })
            }
        </div>
    </div>
}


function MidiLink({ isInputNull, isOutputNull, isActive }: { isInputNull: boolean; isOutputNull: boolean; isActive: boolean; }) {

    if (!isActive) {
        return <div className={`midiTableCell Link FullNull`}>
            <div className={`circle red`}></div>
            <div className={`circle blue`}></div>
        </div>
    }
    return <div className={`midiTableCell Link ${isInputNull ? isOutputNull ? "FullNull" : "LeftNull" : isOutputNull ? "RightNull" : "LeftNote"} `}>
        <div className={`circle ${isInputNull ? "red" : "active-red"}`}></div>
        <div className={`circle ${isOutputNull ? "blue" : "active-blue"}`}></div>
    </div>
}

export default MidiTableComponent