/*
  ==============================================================================

    GameEngine.h
    Created: 26 Sep 2025 6:24:17pm
    Author:  Wolney

  ==============================================================================
*/

#pragma once

#include <memory> // Para std::unique_ptr
#include "NoteSequence.h"
#include "Note.h"
#include "Challenge.h"
#include "MelodyChallenge.h"


class GameEngine
{
public:
    GameEngine();

    void startNewGame(int notes_quantity);
    void nextLevel(); // Avan�a para pr�ximo n�vel sem resetar score
    void processUserInput(const Note& note);
    bool checkUserSequence();
    const NoteSequence& getUserInputSequence() const;
    void clearUserInput(); // M�todo para limpar entrada do usu�rio

    int getScore() const;

    const NoteSequence& getCurrentTargetSequence() const;

private:
    std::unique_ptr<Challenge> currentChallenge;
    NoteSequence userInputSequence;
    int score;
    int current_challenge_size; // Para lembrar o tamanho da sequ�ncia atual para dar devidos pontos
};