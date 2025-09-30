

/*
  ==============================================================================

    NoteSequence.cpp
    Created: 26 Sep 2025 5:35:11pm
    Author:  Wolney

  ==============================================================================
*/

#include "NoteSequence.h"

void NoteSequence::addNote(const Note& note)
{
    notes.push_back(note);
}

void NoteSequence::clear()
{
    notes.clear();
}

size_t NoteSequence::size() const
{
    return notes.size();
}

const std::vector<Note>& NoteSequence::getNotes() const
{
    return notes;
}