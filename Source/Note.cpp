/*
  ==============================================================================

    Note.cpp
    Created: 26 Sep 2025 6:23:04pm
    Author:  Wolney

  ==============================================================================
*/


#include "Note.h"

Note::Note(int midiNote, int noteVelocity)
    : midiNoteNumber(midiNote), velocity(noteVelocity)
{
}

int Note::getMidiNoteNumber() const
{
    return midiNoteNumber;
}

int Note::getVelocity() const
{
    return velocity;
}

bool Note::operator==(const Note& other) const
{
    return midiNoteNumber == other.midiNoteNumber;
}