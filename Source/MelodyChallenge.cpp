/*
  ==============================================================================

    MelodyChallenge.cpp
    Created: 26 Sep 2025 6:24:41pm
    Author:  Wolney

  ==============================================================================
*/
#pragma once

#include "MelodyChallenge.h"
#include "Challenge.h"
#include <random> // Para gerar números aleatórios


void MelodyChallenge::generateSequence(int difficulty) {

    // Limpa sequência anterior
    targetSequence.clear();

    // Lógica para gerar sequência aleatória

    std::random_device rd;
    std::mt19937 gen(rd()); // mt = Mersenne Twister, um gerador de números pseudoaleatórios
    // 19937 é o tamanho do estado do gerador, 19937 é um número primo

    std::uniform_int_distribution<> distrib(60, 72); // Notas MIDI entre C4 (60) e B4 (71)

    for (int i = 0; i < difficulty; ++i) {
        int randomMidiNote = distrib(gen);
        Note note(randomMidiNote, 100); // Velocidade fixa de 100
        targetSequence.addNote(note);
    }

}

bool MelodyChallenge::isCorrect(const NoteSequence& userSequence) const
{
    const auto& targetNotes = targetSequence.getNotes();
    const auto& userNotes = userSequence.getNotes();

    if (targetNotes.size() != userNotes.size()) {
        return false; // Tamanhos diferentes, resposta incorreta
    }
    for (size_t i = 0; i < targetNotes.size(); ++i) {
        if (!(targetNotes[i] == userNotes[i])) {
            return false; // Notas diferentes, resposta incorreta
        }
    }
    return true; // Todas as notas conferem, resposta correta
}
