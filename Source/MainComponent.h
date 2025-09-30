#pragma once

#include <JuceHeader.h>

// Incluí os cabeçalhos das classes que o MainComponent vai conter.
// O compilador vai encontrá-los na pasta "design" porque configurei o caminho.
#include "GameEngine.h"
#include "VirtualKeyboardComponent.h"

//==============================================================================
class MainComponent : public juce::Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    // Aqui declarei que o MainComponent POSSUI um GameEngine e um VirtualKeyboardComponent.
    // Esta é uma relação de COMPOSIÇÃO  
    GameEngine gameEngine;
    VirtualKeyboardComponent keyboardComponent;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent);
};