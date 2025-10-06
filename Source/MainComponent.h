#pragma once

#include <JuceHeader.h>

#include "GameEngine.h"
// A classe virtual keyboard não é mais necessária pois o JUCE tem uma implementação já pronta

//==============================================================================
class MainComponent : public juce::Component, 
                      public juce::MidiInputCallback, // Esta herança serve para reconhecer dispositivos MIDI
                      private juce::Timer, // Timer para animação de flash
                      public juce::AudioSource // Para produzir áudio
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    
    // Esta função será chamada quando uma mensagem MIDI chegar
    void handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message) override;
    
    void paint(juce::Graphics&) override;
    void resized() override;

    // Métodos da interface AudioSource
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

private:
    //==============================================================================
    // Callback do Timer para animação de flash
    void timerCallback() override;
    
    // Função para tocar a sequência do desafio
    void playSequence();
    
    // Esta é uma relação de COMPOSIÇÃO  
    GameEngine gameEngine;
    
    juce::MidiKeyboardState keyboardState; // Para manter o estado do teclado virtual
    juce::MidiKeyboardComponent keyboardComponent; // O componente visual do teclado virtual
    
    // Para guardar o ponteiro para o dispositivo MIDI que está sendo usado
    std::unique_ptr<juce::MidiInput> midiInput;

    // Botões de controle
    juce::TextButton startButton;
    juce::TextButton replayButton;  // Botão para repetir sequência

    // Declaração do label para mostrar a pontuação e selecionar quantidade de notas
    juce::Slider numNotesSlider;
    juce::Label numNotesLabel;

    // Variáveis para o efeito de flash na tela
    juce::Colour flashColour;
    float flashOpacity = 0.0f;

    // ========== COMPONENTES DE ÁUDIO ==========
    juce::AudioDeviceManager audioDeviceManager;
    juce::Synthesiser synth;
    juce::MidiMessageCollector midiCollector;
    juce::AudioSourcePlayer audioSourcePlayer;
    
    // Para tocar a sequência do desafio
    juce::MidiBuffer sequenceBuffer;
    int sequencePosition = 0;
    bool isPlayingSequence = false;
    double sampleRate = 44100.0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent);
};