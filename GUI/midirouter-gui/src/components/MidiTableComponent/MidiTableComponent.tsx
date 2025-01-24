import React from "react";


function MidiTableComponent({ translationTable }: { translationTable: MidiTableEventRow[] }) {

    return <div className="midiTable">
        <div className="midiTableHeader">
            <div className="midiTableHeaderColumnInput"></div>
            <div className="midiTableHeaderColumnOutput"></div>
        </div>
        <div className="midiTableRows">
            {
                translationTable.map(t => {

                    const isInputNull = t.inputMIDI?.trim() === "" || !t.inputMIDI;
                    const isOutputNull = t.outputMIDI?.trim() === "" || !t.outputMIDI;

                    return <div className="midiTableRow">
                        <div className={`midiTableCell Input  ${isInputNull ? "MidiNull" : "MidiNote"}`}>{isInputNull ? ".." : t.inputMIDI}</div>
                        <div className={`midiTableCell Link ${isInputNull ? isOutputNull ? "FullNull" : "LeftNull" : isOutputNull ? "RightNull" : "LeftNote"} `}>
                            <div className={`circle ${isInputNull ? "red" : "active-red"}`}></div>
                            <div className={`circle ${isOutputNull ? "blue" : "active-blue"}`}></div>
                        </div>
                        <div className={`midiTableCell Output  ${isOutputNull ? "MidiNull" : "MidiNote"}`}>{isOutputNull ? ".." : t.outputMIDI}</div>
                    </div>
                })
            }
        </div>
    </div>
}

export default MidiTableComponent