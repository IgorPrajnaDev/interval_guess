/*
  ==============================================================================

    GameEngine.cpp
    Created: 26 Sep 2025 5:34:05pm
    Author:  Wolney

  ==============================================================================
*/

#include "GameEngine.h"
#include "MelodyChallenge.h"

GameEngine::GameEngine() : score(0), current_challenge_size(3) // Inicia com 3 notas
{
    // Se eu quisesse iniciar o jogo automaticamente, poderia chamar startNewGame() aqui
}

void GameEngine::startNewGame(int notes_quantity)
{
    score = 0; // Reseta score apenas quando inicio novo jogo
    current_challenge_size = notes_quantity;

    userInputSequence.clear(); // Limpa a sequ�ncia tocada anteriormente

    currentChallenge = std::make_unique<MelodyChallenge>(); // GameEngine tem um �nico MelodyChallenge

    currentChallenge->generateSequence(current_challenge_size); 
}

void GameEngine::nextLevel()
{
    // N�O incremento mais o tamanho - mantenho o mesmo n�vel
    // current_challenge_size++; // REMOVIDO - sem progress�o autom�tica
    
    userInputSequence.clear(); // Limpa a sequ�ncia tocada anteriormente

    currentChallenge = std::make_unique<MelodyChallenge>(); // GameEngine tem um �nico MelodyChallenge

    currentChallenge->generateSequence(current_challenge_size); 
}

bool GameEngine::checkUserSequence()
{
    if (!currentChallenge)
        return false; // Nenhum desafio ativo
    bool correct = currentChallenge->isCorrect(userInputSequence);
    if (correct)
    {
        score += current_challenge_size * 10; // Exemplo: 10 pontos por nota correta
        // REMOVIDO: current_challenge_size++; 
        // O incremento agora acontece em nextLevel()
    }
    else
    {
        score -= 5; // Penalidade por erro
        if (score < 0) score = 0; // Evita pontua��o negativa
    }
    
    return correct;
}

void GameEngine::processUserInput(const Note& note)
{
    userInputSequence.addNote(note);
}

void GameEngine::clearUserInput()
{
    userInputSequence.clear();
}

int GameEngine::getScore() const
{
    return score;
}

const NoteSequence& GameEngine::getUserInputSequence() const
{
    return userInputSequence;
}

const NoteSequence& GameEngine::getCurrentTargetSequence() const
{
    // Garanto que n�o tento acessar um desafio que n�o existe.
    if (currentChallenge)
        return currentChallenge->getTargetSequence();

    // Se n�o houver desafio ativo, retorno uma sequ�ncia vazia segura
    // para evitar que o programa crashe.
    static const NoteSequence emptySequence;
    return emptySequence;
}
// ------------------------------------