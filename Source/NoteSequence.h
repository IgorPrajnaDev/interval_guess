/*
  ==============================================================================

    NoteSequence.h
    Created: 26 Sep 2025 6:23:29pm
    Author:  Wolney

  ==============================================================================
*/


#pragma once

#include <vector>
#include "Note.h"

class NoteSequence
{
public:
    NoteSequence() = default;

    void addNote(const Note& note);
    void clear();
    size_t size() const;

    const std::vector<Note>& getNotes() const;

private:
    std::vector<Note> notes;
};