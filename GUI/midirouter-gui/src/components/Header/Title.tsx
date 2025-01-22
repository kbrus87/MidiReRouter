import React from "react";


function Title({ text, id, className, span, spanclassName }: { text: string; id?: string; className?: string; span?: string; spanclassName?: string; }) {

    return <div id={id} className={className}>{text} {span ? <span className={spanclassName}>{span}</span> : ""}</div>
}

export default Title