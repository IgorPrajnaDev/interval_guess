#include "MainComponent.h"
#include "Note.h"
//==============================================================================
MainComponent::MainComponent() : keyboardComponent(keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{   

	addAndMakeVisible(keyboardComponent); // Adiciona o teclado virtual à interface

    // Pega a lista de dispositivos de entrada MIDI disponíveis.
    auto midiInputs = juce::MidiInput::getAvailableDevices();

        // Se encontrarmos pelo menos um dispositivo...
    if (midiInputs.size() > 0)
    {
        // Pega o primeiro dispositivo da lista.
        auto firstDevice = midiInputs[0];
        
        // Tenta abrir o dispositivo para escuta.
        // O 'this' diz ao dispositivo para enviar as mensagens para o nosso MainComponent.
        midiInput = juce::MidiInput::openDevice(firstDevice.identifier, this);

        if (midiInput)
        {
            midiInput->start(); // Inicia a escuta de mensagens.
            DBG("Dispositivo MIDI aberto: " + firstDevice.name);
        }
        else
        {
            DBG("Falha ao abrir dispositivo MIDI.");
        }
        }
        else
        {
        DBG("Nenhum dispositivo MIDI de entrada encontrado.");
        }
    

    setSize (800, 600);
}





    // o metodo destrutor de maincomponent desativa o dispositivo!
MainComponent::~MainComponent()
{
    if (midiInput)
    {
        midiInput->stop();
        midiInput = nullptr; // Fecha o dispositivo
    }
}
    


//==============================================================================

    void MainComponent::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message)
    {   
        /* envia a mensagem para a classe que gerencia o estado do teclado
        esta já atualiza a parte visual */
		keyboardState.processNextMidiEvent(message);

        // Nós estamos interessados em mensagens de noteon (tecla pressionada)
        if (message.isNoteOn())
        {
        // Extraindo informaçoes do evento midi
        int midiNoteNumber = message.getNoteNumber();
        int velocity = message.getVelocity();

        // Imprimimos no console de debug para ver se está funcionando
        DBG("Note On: " << midiNoteNumber << " Velocity: " << velocity);

        // =================================================================
        // AQUI É A CONEXÃO COM A LÓGICA DO JOGO
        // Criamos um objeto Note e o passamos para o GameEngine. 
        gameEngine.processUserInput(Note(midiNoteNumber, velocity));
        // =================================================================
        }
    // Também podemos checar por mensagens de "Nota Desligada" (tecla solta)
    else if (message.isNoteOff())
    {
        int midiNoteNumber = message.getNoteNumber();
        DBG("Note Off: " << midiNoteNumber);
    }
    }

void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont (juce::FontOptions (16.0f));
    g.setColour (juce::Colours::green);
    g.drawText ("Meloquiz", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    auto area = getLocalBounds();
    keyboardComponent.setBounds(area.removeFromBottom(150));
}
