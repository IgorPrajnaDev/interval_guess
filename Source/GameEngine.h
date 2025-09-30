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


class GameEngine
{
public:
    GameEngine();

    void startNewGame();
    void nextLevel();
    void processUserInput(const Note& note);
    bool checkUserSequence();

    int getScore() const;
    int getLevel() const;
    const NoteSequence& getCurrentTargetSequence() const;

private:
    std::unique_ptr<Challenge> currentChallenge;
    NoteSequence userInputSequence;
    int score;
    int level;
};