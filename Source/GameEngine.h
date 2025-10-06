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
    void nextLevel(); // Avança para próximo nível sem resetar score
    void processUserInput(const Note& note);
    bool checkUserSequence();
    const NoteSequence& getUserInputSequence() const;
    void clearUserInput(); // Método para limpar entrada do usuário

    int getScore() const;

    const NoteSequence& getCurrentTargetSequence() const;

private:
    std::unique_ptr<Challenge> currentChallenge;
    NoteSequence userInputSequence;
    int score;
    int current_challenge_size; // Para lembrar o tamanho da sequência atual para dar devidos pontos
};