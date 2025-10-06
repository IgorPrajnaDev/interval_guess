#include <JuceHeader.h>
#include "MainComponent.h"
#include "Note.h"
#include "SimpleSineVoice.h"

//==============================================================================
MainComponent::MainComponent() : keyboardComponent(keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard),
                                 midiCollector()
{
    // --- CONFIGURAÇÃO DO SINTETIZADOR DE ÁUDIO ---
    
    // Adiciono 8 vozes de sintetizador de ondas seno
    for (int i = 0; i < 8; ++i)
        synth.addVoice(new SimpleSineVoice());
    
    // Adiciono um som que abrange todas as notas MIDI
    synth.addSound(new SimpleSineSound());
    
    // Configuro o áudio
    juce::String error = audioDeviceManager.initialiseWithDefaultDevices(0, 2);
    if (error.isNotEmpty())
    {
        DBG("Erro ao inicializar áudio: " + error);
    }
    
    audioDeviceManager.addAudioCallback(&audioSourcePlayer);
    audioSourcePlayer.setSource(this);

    // --- CONFIGURAÇÃO DOS COMPONENTES FEITA AQUI, NO CONSTRUTOR ---

    addAndMakeVisible(keyboardComponent); // Adiciono o teclado virtual à interface

    // Configuro o botão de iniciar
    addAndMakeVisible(startButton);
    startButton.setButtonText("Iniciar Novo Jogo");

    // Configuro o botão de repetir sequência
    addAndMakeVisible(replayButton);
    replayButton.setButtonText("Repetir Sequencia");
    replayButton.setEnabled(false); // Desabilitado até iniciar um jogo
    replayButton.onClick = [this]() {
        // Limpo a entrada do usuário ao repetir
        gameEngine.clearUserInput();
        // Toco a sequência novamente
        playSequence();
        repaint();
    };

    addAndMakeVisible(numNotesSlider);
    numNotesSlider.setRange(2, 12, 1); // De 2 a 12 notas, com passo de 1
    numNotesSlider.setValue(4);
    numNotesSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 40, 20);

    // Configuro o Label (texto) para o slider
    addAndMakeVisible(numNotesLabel);

    // Modifico a ação do botão
    startButton.onClick = [this]() {
        // Pego o valor atual do slider
        int numNotes = numNotesSlider.getValue();
        // Inicio o jogo com essa quantidade de notas
        gameEngine.startNewGame(numNotes);
        flashOpacity = 0.0f; // Reseto o flash ao iniciar novo jogo
        stopTimer(); // Paro qualquer animação em andamento
        
        // Habilito o botão de replay
        replayButton.setEnabled(true);
        
        // Debug: Mostro a sequência gerada
        const auto& targetSequence = gameEngine.getCurrentTargetSequence().getNotes();
        juce::String sequenceDebug = "Sequencia gerada: ";
        for (const auto& note : targetSequence)
        {
            sequenceDebug += juce::String(note.getMidiNoteNumber()) + " ";
        }
        DBG(sequenceDebug);
        
        // Toco a sequência do desafio
        playSequence();
        
        repaint();
    };

    // Pego a lista de dispositivos de entrada MIDI disponíveis.
    auto midiInputs = juce::MidiInput::getAvailableDevices();

    // Se encontro pelo menos um dispositivo...
    if (midiInputs.size() > 0)
    {
        // Pego o primeiro dispositivo da lista.
        auto firstDevice = midiInputs[0];

        // Tento abrir o dispositivo para escuta.
        // O 'this' diz ao dispositivo para enviar as mensagens para o meu MainComponent.
        midiInput = juce::MidiInput::openDevice(firstDevice.identifier, this);

        if (midiInput)
        {
            midiInput->start(); // Inicio a escuta de mensagens.
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

// O método destrutor de MainComponent desativa o dispositivo!
MainComponent::~MainComponent()
{
    audioSourcePlayer.setSource(nullptr);
    audioDeviceManager.removeAudioCallback(&audioSourcePlayer);
    
    if (midiInput)
    {
        midiInput->stop();
        midiInput = nullptr; // Fecho o dispositivo
    }
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double newSampleRate)
{
    sampleRate = newSampleRate;
    synth.setCurrentPlaybackSampleRate(newSampleRate);
    midiCollector.reset(newSampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    bufferToFill.clearActiveBufferRegion();
    
    juce::MidiBuffer incomingMidi;
    midiCollector.removeNextBlockOfMessages(incomingMidi, bufferToFill.numSamples);
    
    // Buffer separado para as notas do desafio (não será enviado ao keyboardState)
    juce::MidiBuffer challengeMidi;
    
    // Se estou tocando a sequência, adiciono as notas ao buffer MIDI do desafio
    if (isPlayingSequence && !sequenceBuffer.isEmpty())
    {
        challengeMidi.addEvents(sequenceBuffer, 0, bufferToFill.numSamples, 0);
        
        // Removo as mensagens que já foram tocadas
        juce::MidiBuffer remainingBuffer;
        for (const auto metadata : sequenceBuffer)
        {
            if (metadata.samplePosition >= bufferToFill.numSamples)
            {
                remainingBuffer.addEvent(metadata.getMessage(), 
                                        metadata.samplePosition - bufferToFill.numSamples);
            }
        }
        sequenceBuffer = remainingBuffer;
        
        if (sequenceBuffer.isEmpty())
            isPlayingSequence = false;
    }
    
    // IMPORTANTE: Apenas as notas do usuário vão para o keyboardState (animação visual)
    keyboardState.processNextMidiBuffer(incomingMidi, 0, bufferToFill.numSamples, true);
    
    // Combino as notas do usuário E do desafio para o sintetizador (som)
    juce::MidiBuffer combinedMidi;
    combinedMidi.addEvents(incomingMidi, 0, bufferToFill.numSamples, 0);
    combinedMidi.addEvents(challengeMidi, 0, bufferToFill.numSamples, 0);
    
    // Renderizo o áudio com AMBAS as notas
    synth.renderNextBlock(*bufferToFill.buffer, combinedMidi, 0, bufferToFill.numSamples);
}

void MainComponent::releaseResources()
{
    // Liberação de recursos quando o áudio para
}

void MainComponent::playSequence()
{
    const auto& targetSequence = gameEngine.getCurrentTargetSequence().getNotes();
    
    if (targetSequence.empty())
        return;
    
    sequenceBuffer.clear();
    
    // Crio mensagens MIDI para cada nota da sequência
    int sampleOffset = 0;
    const int noteDurationSamples = static_cast<int>(sampleRate * 0.5); // 500ms por nota
    const int noteGapSamples = static_cast<int>(sampleRate * 0.2); // 200ms de pausa entre notas
    
    for (const auto& note : targetSequence)
    {
        // Mensagem de Note On
        juce::MidiMessage noteOn = juce::MidiMessage::noteOn(1, note.getMidiNoteNumber(), (juce::uint8)100);
        sequenceBuffer.addEvent(noteOn, sampleOffset);
        
        // Mensagem de Note Off
        juce::MidiMessage noteOff = juce::MidiMessage::noteOff(1, note.getMidiNoteNumber());
        sequenceBuffer.addEvent(noteOff, sampleOffset + noteDurationSamples);
        
        sampleOffset += noteDurationSamples + noteGapSamples;
    }
    
    isPlayingSequence = true;
}

//==============================================================================
void MainComponent::handleIncomingMidiMessage(juce::MidiInput* source, const juce::MidiMessage& message)
{
    // CORREÇÃO: Envio o evento MIDI para o coletor de forma thread-safe
    midiCollector.addMessageToQueue(message);
    
    // Atualizo o estado do teclado de forma thread-safe
    keyboardState.processNextMidiEvent(message);

    // Estou interessado em mensagens de noteOn (tecla pressionada)
    if (message.isNoteOn())
    {
        // Extraio informações do evento MIDI
        int midiNoteNumber = message.getNoteNumber();
        int velocity = message.getVelocity();

        // Imprimo no console de debug para ver se está funcionando
        DBG("Note On: " << midiNoteNumber << " Velocity: " << velocity);

        // =================================================================
        // AQUI É A CONEXÃO COM A LÓGICA DO JOGO
        // Crio um objeto Note e o passo para o GameEngine.  
        gameEngine.processUserInput(Note(midiNoteNumber, velocity));
        // =================================================================

        // --- LÓGICA ADICIONADA PARA VERIFICAR A JOGADA ---
        const auto numNotesInChallenge = gameEngine.getCurrentTargetSequence().getNotes().size();
        // Garanto que só verifico se o desafio já começou (numNotesInChallenge > 0)
        if (numNotesInChallenge > 0 && gameEngine.getUserInputSequence().getNotes().size() >= numNotesInChallenge)
        {
            bool correct = gameEngine.checkUserSequence(); // Verifico se a resposta está certa
            
            // CORREÇÃO: Uso MessageManager::callAsync para atualizar a GUI de forma thread-safe
            juce::MessageManager::callAsync([this, correct]() {
                // Inicio o efeito de flash na tela
                flashColour = correct ? juce::Colours::green : juce::Colours::red;
                flashOpacity = 0.7f; // Opacidade inicial (70%)
                startTimer(30); // Atualizo a cada 30ms (~33 fps)
                
                repaint(); // Mando a tela se redesenhar para mostrar o novo placar/desafio
                
                // Se acertou, toco a próxima sequência automaticamente
                if (correct)
                {
                    // Aguardo um pouco antes de tocar a próxima
                    juce::Timer::callAfterDelay(1500, [this]() {
                        // CORRIGIDO: Uso nextLevel() em vez de startNewGame() para manter o score
                        gameEngine.nextLevel();
                        playSequence();
                        repaint();
                    });
                }
                else
                {
                    // Se errou, limpo a entrada do usuário após o flash
                    // para permitir nova tentativa
                    juce::Timer::callAfterDelay(1000, [this]() {
                        gameEngine.clearUserInput();
                        repaint();
                    });
                }
            });
        }
        // --- FIM DA LÓGICA ADICIONADA ---
    }
    // Também posso checar por mensagens de "Nota Desligada" (tecla solta)
    else if (message.isNoteOff())
    {
        int midiNoteNumber = message.getNoteNumber();
        DBG("Note Off: " << midiNoteNumber);
    }
}

//==============================================================================
void MainComponent::paint(juce::Graphics& g)
{
    // Preencho completamente o fundo com uma cor sólida
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setFont(juce::Font(30.0f));
    g.setColour(juce::Colours::white);

    /*recebendo os dados do gameEngine contido*/

    auto score = gameEngine.getScore();
    const auto& challengeNotes = gameEngine.getCurrentTargetSequence().getNotes();
    const auto& userNotes = gameEngine.getUserInputSequence().getNotes();

    /* criando as strings juce que serao exibidas */
    juce::String scoreText = "Score: " + juce::String(score);

    /*isso desenha o placar e o nivel na tela */

    // Usando o metodo explicito com coordenadas que sabemos que funciona,
    // em vez dos metodos que estavam a dar erro.
    g.drawText(scoreText, 20, 60, 200, 30, juce::Justification::topLeft, true);

    if (!challengeNotes.empty())
    {
        // Mostro o texto no centro
        if (isPlayingSequence)
        {
            g.setColour(juce::Colours::yellow);
            g.drawText("Ouvindo a sequencia...", getLocalBounds(), juce::Justification::centred, true);
        }
        else
        {
            g.setColour(juce::Colours::white);
            g.drawText("Repita a sequencia", getLocalBounds(), juce::Justification::centred, true);
        }
        
        // Mostro o progresso da entrada do usuario
        g.setColour(juce::Colours::white);
        g.setFont(juce::Font(20.0f));
        juce::String progressText = "Notas: " + juce::String(userNotes.size()) + " / " + juce::String(challengeNotes.size());
        g.drawText(progressText, 20, 20, 200, 30, juce::Justification::topLeft, true);
    }

    // Desenho o overlay de flash se a opacidade for maior que 0
    if (flashOpacity > 0.0f)
    {
        g.setColour(flashColour.withAlpha(flashOpacity));
        g.fillAll();
    }
}

void MainComponent::resized()
{
    // --- MÉTODO RESIZED CORRIGIDO ---
    auto area = getLocalBounds();

    // Crio uma área no topo para os controles
    auto controlsArea = area.removeFromTop(100);
    
    // Divido a área de controles em duas colunas
    auto leftControls = controlsArea.removeFromLeft(controlsArea.getWidth() / 2);
    auto rightControls = controlsArea;
    
    // Lado esquerdo: Botão de iniciar
    startButton.setBounds(leftControls.removeFromTop(60).reduced(20));
    
    // Lado direito: Botão de replay
    replayButton.setBounds(rightControls.removeFromTop(60).reduced(20));

    // Posiciono o slider abaixo dos botões (área que sobrou)
    auto sliderArea = area.removeFromTop(40);
    numNotesSlider.setBounds(sliderArea.withLeft(150).reduced(20, 0));

    // Posiciono o teclado na área inferior
    keyboardComponent.setBounds(area.removeFromBottom(150));
}

//==============================================================================
void MainComponent::timerCallback()
{
    // Reduzo a opacidade gradualmente (esmaecimento)
    flashOpacity -= 0.04f; // Diminuo ~4% por frame
    
    if (flashOpacity <= 0.0f)
    {
        flashOpacity = 0.0f;
        stopTimer(); // Paro o timer quando o flash termina
    }
    
    repaint(); // Redesenho para mostrar a nova opacidade
}