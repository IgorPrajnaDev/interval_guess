#include <JuceHeader.h>
#include "MainComponent.h"
#include "Note.h"

//==============================================================================
MainComponent::MainComponent() : keyboardComponent(keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard)
{
    // --- CONFIGURAÇÃO DOS COMPONENTES FEITA AQUI, NO CONSTRUTOR ---

    addAndMakeVisible(keyboardComponent); // Adiciona o teclado virtual à interface

    // Configura o botão de iniciar, que estava faltando
    addAndMakeVisible(startButton);
    startButton.setButtonText("Iniciar Novo Jogo");

    addAndMakeVisible(numNotesSlider);
    numNotesSlider.setRange(3, 12, 1); // De 3 a 12 notas, com passo de 1
    numNotesSlider.setValue(4);
    numNotesSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 40, 20);

    // Configura o Label (texto) para o slider
    addAndMakeVisible(numNotesLabel);


    // Modifica a ação do botão
    startButton.onClick = [this]() {
        // Pega o valor atual do slider
        int numNotes = numNotesSlider.getValue();
        // Inicia o jogo com essa quantidade de notas
        gameEngine.startNewGame(numNotes);
        repaint();
        };

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

    setSize(800, 600);
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

        // --- LÓGICA ADICIONADA PARA VERIFICAR A JOGADA ---
        const auto numNotesInChallenge = gameEngine.getCurrentTargetSequence().getNotes().size();
        // Garante que só verificamos se o desafio já começou (numNotesInChallenge > 0)
        if (numNotesInChallenge > 0 && gameEngine.getUserInputSequence().getNotes().size() >= numNotesInChallenge)
        {
            gameEngine.checkUserSequence(); // Verifica se a resposta está certa
            repaint(); // Manda a tela se redesenhar para mostrar o novo placar/desafio
        }
        // --- FIM DA LÓGICA ADICIONADA ---
    }
    // Também podemos checar por mensagens de "Nota Desligada" (tecla solta)
    else if (message.isNoteOff())
    {
        int midiNoteNumber = message.getNoteNumber();
        DBG("Note Off: " << midiNoteNumber);
    }
}

void MainComponent::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setFont(juce::Font(30.0f));
    g.setColour(juce::Colours::white);

    /*
    g.drawText ("Meloquiz", getLocalBounds(), juce::Justification::centred, true);
    */

    /*recebendo os dados do gameEngine contido*/

    auto score = gameEngine.getScore();
    const auto& challengeNotes = gameEngine.getCurrentTargetSequence().getNotes();

    /* criando as strings juce que serao exibidas */
    juce::String scoreText = "Score: " + juce::String(score);


    /*isso desenha o placar e o nivel na tela */

    // Usando o método explícito com coordenadas que sabemos que funciona,
    // em vez dos métodos que estavam a dar erro.
    const int windowWidth = getWidth();
    g.drawText(scoreText, 20, 60, 200, 30, juce::Justification::topLeft, true);


    if (!challengeNotes.empty())
    {
        g.drawText("Repita", getLocalBounds(), juce::Justification::centred, true);
    }
}

void MainComponent::resized()
{
    // --- MÉTODO RESIZED CORRIGIDO ---
    auto area = getLocalBounds();

    // Cria uma área no topo para os controlos
    auto controlsArea = area.removeFromTop(100);
    startButton.setBounds(controlsArea.removeFromTop(60).reduced(20));

    // Posiciona o slider na área restante dos controlos
    numNotesSlider.setBounds(controlsArea.withLeft(150).reduced(20, 0));

    // Posiciona o teclado na área inferior
    keyboardComponent.setBounds(area.removeFromBottom(150));
}