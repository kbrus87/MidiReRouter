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
                    return  <div className="midiTableRow">
                                <div className="midiTableCell Input">{t.inputMIDI}</div>
                                <div className="midiTableCell Link"></div>
                                <div className="midiTableCell Output">{t.outputMIDI}</div>
                            </div>
                })
            }
        </div>
    </div>
}

export default MidiTableComponent