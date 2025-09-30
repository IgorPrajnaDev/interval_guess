/*
  ==============================================================================

    Challenge.cpp
    Created: 26 Sep 2025 6:23:14pm
    Author:  Wolney

  ==============================================================================
*/


#include "Challenge.h"

// Como Challenge é uma classe abstrata, não implementamos os métodos virtuais puros.
// Apenas os métodos que já têm uma implementação.

const NoteSequence& Challenge::getTargetSequence() const
{
    return targetSequence;
}