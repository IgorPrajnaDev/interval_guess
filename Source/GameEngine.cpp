
/*
  ==============================================================================

    GameEngine.cpp
    Created: 26 Sep 2025 5:34:05pm
    Author:  Wolney

  ==============================================================================
*/

#include "GameEngine.h"
#include "MelodyChallenge.h"

GameEngine::GameEngine() : score(0), current_challenge_size(3) //inicia com 3 notas
{
	// se eu quisesse iniciar o jogo automaticamente, poderia chamar startNewGame() aqui
}

void GameEngine::startNewGame(int notes_quantity)
{
	score = 0;
	current_challenge_size = notes_quantity;

	userInputSequence.clear(); //limpa a sequencia tocada anteriormente


	currentChallenge = std::make_unique<MelodyChallenge>(); //gameengine tem um unico melodychallenge

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
		current_challenge_size++; //aumenta o tamanho da proxima sequencia
	}
	else
	{
		score -= 5; // Penalidade por erro
		if (score < 0) score = 0; // Evita pontuação negativa
	}
	
	return correct;
}

void GameEngine::processUserInput(const Note& note)
{
	userInputSequence.addNote(note);
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
	// Garante que não tentamos aceder a um desafio que não existe.
	if (currentChallenge)
		return currentChallenge->getTargetSequence();

	// Se não houver desafio ativo, retorna uma sequência vazia segura
	// para evitar que o programa crashe.
	static const NoteSequence emptySequence;
	return emptySequence;
}
// ------------------------------------