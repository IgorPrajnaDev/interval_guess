/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

// A "regra de ouro": JuceHeader.h e os cabeçalhos do próprio componente primeiro.
#include <JuceHeader.h>
#include "MainComponent.h"

// Agora, os includes para arquitetura customizada.

#include "Challenge.h"
#include "GameEngine.h"
#include "MelodyChallenge.h"
#include "Note.h"
#include "NoteSequence.h"
#include "VirtualKeyboardComponent.h"

using namespace juce;


//==============================================================================
class interval_guessApplication : public juce::JUCEApplication
{
public:
    //==============================================================================
    interval_guessApplication() {}

    const juce::String getApplicationName() override { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion() override { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override { return true; }

    //==============================================================================
    void initialise(const juce::String& commandLine) override
    {
        mainWindow.reset(new MainWindow(getApplicationName()));
    }

    void shutdown() override
    {
        mainWindow = nullptr; // Deleta a janela
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        quit();
    }

    void anotherInstanceStarted(const juce::String& commandLine) override
    {
    }

    //==============================================================================
    class MainWindow : public juce::DocumentWindow
    {
    public:
        MainWindow(juce::String name)
            : DocumentWindow(name,
                juce::Desktop::getInstance().getDefaultLookAndFeel()
                .findColour(juce::ResizableWindow::backgroundColourId),
                DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(new MainComponent(), true);

#if JUCE_IOS || JUCE_ANDROID
            setFullScreen(true);
#else
            setResizable(true, true);
            centreWithSize(getWidth(), getHeight());
#endif

            setVisible(true);
        }

        void closeButtonPressed() override
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// Esta macro gera a rotina main() que inicia a aplicação.
START_JUCE_APPLICATION(interval_guessApplication)