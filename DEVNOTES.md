#####

## How to binaryData
Make sure you build the react app at
\GUI\midirouter-gui
    npm install
    nmp run build
at \GUI\cmakebuild
    cmake -S .. -B . -G "Visual Studio 17 2022"
    cmake --build .

then manually move .cpp and .h from  
    GUI/cmakebuild/juce_binarydata_MyBinaryData/JuceLibraryCode 
    to
    GUI/binary