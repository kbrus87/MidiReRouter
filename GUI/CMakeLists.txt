cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
project(GenerateBinaryData LANGUAGES CXX)


set(JUCE_DIR "D://JUCE-SOURCE")  # Ajustá el path si hace falta
add_subdirectory(${JUCE_DIR} JUCE)

set(WEBVIEW_FILES_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/midirouter-gui/build")

file(GLOB_RECURSE STATIC_JS_FILES "${WEBVIEW_FILES_SOURCE_DIR}/static/js/*")
file(GLOB_RECURSE STATIC_CSS_FILES "${WEBVIEW_FILES_SOURCE_DIR}/static/css/*")
file(GLOB_RECURSE STATIC_MEDIA_FILES "${WEBVIEW_FILES_SOURCE_DIR}/static/media/*")

juce_add_binary_data(MyBinaryData
    HEADER_NAME MyBinaryData.h
    NAMESPACE frontend
    SOURCES
        "${WEBVIEW_FILES_SOURCE_DIR}/index.html"
        "${WEBVIEW_FILES_SOURCE_DIR}/asset-manifest.json"
        ${STATIC_JS_FILES}
        ${STATIC_CSS_FILES}
        ${STATIC_MEDIA_FILES}
)