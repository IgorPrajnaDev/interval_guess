

/*
  ==============================================================================

    Note.h
    Created: 26 Sep 2025 5:07:57pm
    Author:  Wolney

  ==============================================================================
*/



#pragma once

class Note
{
public:
    // Construtor
    Note(int midiNote, int noteVelocity);

    // Getters
    int getMidiNoteNumber() const;
    int getVelocity() const;

    // Sobrecarga de operador para comparar se duas notas são iguais
    bool operator==(const Note& other) const;

private:
    int midiNoteNumber;
    int velocity;
};