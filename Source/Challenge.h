/*
  ==============================================================================

    Challenge.h
    Created: 26 Sep 2025 6:23:14pm
    Author:  Wolney

  ==============================================================================
*/
#pragma once

#include "NoteSequence.h"

class Challenge
{
public:
    // Destrutor virtual obrigatório em classes base com funções virtuais
    virtual ~Challenge() = default;

    // Métodos virtuais puros que as classes derivadas devem implementar
    virtual void generateSequence(int difficulty) = 0;
    virtual bool isCorrect(const NoteSequence& userSequence) const = 0;

    const NoteSequence& getTargetSequence() const;

protected:
    NoteSequence targetSequence;
};