cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
project(GenerateBinaryData LANGUAGES CXX)


# Ruta principal
set(JUCE_DIR "D:/JUCE-SOURCE")

# Fallback
if(NOT EXISTS ${JUCE_DIR})
    message(WARNING "No se encontró JUCE en ${JUCE_DIR}, intentando ruta C:/JUCE ...")
    set(JUCE_DIR "C:/JUCE")  # Cambiá esto por el path alternativo que quieras
endif()

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


### All of this just to copy files, wow!

# Directorio generado por JUCE
set(GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/juce_binarydata_MyBinaryData/JuceLibraryCode")
set(TARGET_DIR "${CMAKE_CURRENT_SOURCE_DIR}/binary")

# Buscar todos los archivos, excepto los que se llaman input_file_list*
file(GLOB ALL_GENERATED_FILES "${GENERATED_DIR}/*")

set(FILES_TO_COPY "")
foreach(file ${ALL_GENERATED_FILES})
    get_filename_component(name ${file} NAME)
    if(NOT name MATCHES "^input_file_list")
        list(APPEND FILES_TO_COPY ${file})
    endif()
endforeach()

# Custom command para limpiar y copiar
add_custom_command(
    TARGET MyBinaryData POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${TARGET_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${TARGET_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${FILES_TO_COPY}
        ${TARGET_DIR}
    COMMENT "Limpiando GUI/binary/ y copiando archivos generados (excluyendo input_file_list)"
)