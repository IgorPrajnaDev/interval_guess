#pragma once

#include <JuceHeader.h>

#include "GameEngine.h"
// A classe virtual keyboard n�o � mais necess�ria pois o JUCE tem uma implementa��o j� pronta

//==============================================================================
class MainComponent : public juce::Component, 
                      public juce::MidiInputCallback, // Esta heran�a serve para reconhecer dispositivos MIDI
                      private juce::Timer, // Timer para anima��o de flash
                      public juce::AudioSource // Para produzir �udio
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    
    // Esta fun��o ser� chamada quando uma mensagem MIDI chegar
    void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;
    
    void paint(juce::Graphics&) override;
    void resized() override;

    // M�todos da interface AudioSource
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

private:
    //==============================================================================
    // Callback do Timer para anima��o de flash
    void timerCallback() override;
    
    // Fun��o para tocar a sequ�ncia do desafio
    void playSequence();
    
    // Esta � uma rela��o de COMPOSI��O  
    GameEngine gameEngine;
    
    juce::MidiKeyboardState keyboardState; // Para manter o estado do teclado virtual
    juce::MidiKeyboardComponent keyboardComponent; // O componente visual do teclado virtual
    
    // Para guardar o ponteiro para o dispositivo MIDI que est� sendo usado
    std::unique_ptr<juce::MidiInput> midiInput;

    // Bot�es de controle
    juce::TextButton startButton;
    juce::TextButton replayButton;  // Bot�o para repetir sequ�ncia

    // Declara��o do label para mostrar a pontua��o e selecionar quantidade de notas
    juce::Slider numNotesSlider;
    juce::Label numNotesLabel;

    // Vari�veis para o efeito de flash na tela
    juce::Colour flashColour;
    float flashOpacity = 0.0f;

    // ========== COMPONENTES DE �UDIO ==========
    juce::AudioDeviceManager audioDeviceManager;
    juce::Synthesiser synth;
    juce::MidiMessageCollector midiCollector;
    juce::AudioSourcePlayer audioSourcePlayer;
    
    // Para tocar a sequ�ncia do desafio
    juce::MidiBuffer sequenceBuffer;
    int sequencePosition = 0;
    bool isPlayingSequence = false;
    double sampleRate = 44100.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent);
};