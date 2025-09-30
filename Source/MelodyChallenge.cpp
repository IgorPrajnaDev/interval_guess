/*
  ==============================================================================

    MelodyChallenge.cpp
    Created: 26 Sep 2025 6:24:41pm
    Author:  Wolney

  ==============================================================================
*/
#pragma once

#include "Challenge.h"

class MelodyChallenge : public Challenge
{
public:
    // A palavra-chave 'override' garante que estamos sobrescrevendo um m	do virtual
    void generateSequence(int difficulty) override;
    bool isCorrect(const NoteSequence& userSequence) const override;
};