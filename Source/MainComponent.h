#pragma once

#include <JuceHeader.h>

#include "GameEngine.h"
// a classe virtual keyaboard não é mais necessaria pois o juce tem uma implementaçao já pronta

//==============================================================================
class MainComponent : public juce::Component, public juce::MidiInputCallback // essa herança serve pra reconhecer dispositivos midi
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    
    //essa funcao será chamada quando uma mensagem midi chegar
    void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;
    
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    //==============================================================================
  
    // Esta é uma relação de COMPOSIÇÃO  
    GameEngine gameEngine;
    
	juce::MidiKeyboardState keyboardState; //para manter o estado do teclado virtual
	juce::MidiKeyboardComponent keyboardComponent; // o componente visual do teclado virtual
    
    //para guardar o ponteiro para o dispositivo midi que está sendo usado
    
    std::unique_ptr<juce::MidiInput> midiInput;

    // declaraçao do botao de start

    juce::TextButton startButton;

	// declaraçao do label para mostrar a pontuaçao e selecionar quantidade notas
    juce::Slider numNotesSlider;
    juce::Label numNotesLabel;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent);
};