/*
  ==============================================================================

    VirtualKeyboardComponent.h
    Created: 26 Sep 2025 6:24:00pm
    Author:  Wolney

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class VirtualKeyboardComponent : public juce::Component
{
public:
    VirtualKeyboardComponent();
    ~VirtualKeyboardComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    // M	do para a lgica externa (MainComponent) sinalizar uma tecla pressionada
    void setKeyPressed(int midiNote, bool isDown);

private:
    // Aqui vocpode usar um std::map ou std::vector para guardar o estado das teclas
    std::vector<bool> keyStates; // Exemplo: 128 teclas MIDI

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VirtualKeyboardComponent)
};