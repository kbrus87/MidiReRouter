import React from "react";
import Title from "./Title";

function Header() {
    const handleOnClick = () => {
        window.open("https://kbrus87.github.io/#/midiRouter/", "_blank")
    }
    return <div className="header">
        <div onClick={handleOnClick} className="name-block">
            <Title text="Midi Router" className="name align-left"></Title>
        </div>
        <div onClick={handleOnClick} className="logo-block">
            <Title text="by" span="BJR" spanclassName="bjr" className="logo-upside align-right"></Title>
            <Title text="garbageAudio.com" className="logo-downside align-right"></Title>
        </div>
    </div>
}


export default Header