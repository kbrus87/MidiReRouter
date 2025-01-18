/*
  ==============================================================================

    Listener.h
    Created: 18 Jan 2025 6:15:39pm
    Author:  Bruno Ramari

  ==============================================================================
*/

#pragma once

#include "types.h" // Para usar TranslationMidiTable y otros tipos

class Listener
{
public:
    virtual ~Listener() = default;

    // M	do virtual puro para manejar eventos con payload
    virtual void onEvent(const std::string& identifier, const std::variant<int, std::string, TranslationMidiTable>& payload) = 0;
};