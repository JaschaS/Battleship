#ifndef AI_PLAYER_H
#define AI_PLAYER_H

/*	FIT2049 - Assignment 1
*	AIPlayer.h
*	Created by Jascha Scholz - 2015 - Monash Univeristy
*	Simple AI for the battleship game.
*/

#include "GameBoard.h"

class AIPlayer
{
private:
	/**
	 * Board of the ai.
	*/
	GameBoard *m_board;

	/**
	 * Board of the player.
	*/
	GameBoard *m_enemyBoard;

	
public:
	AIPlayer ();

	/**
	 * Initialise the ai.
	*/
	bool Initialise ( GameBoard *board, GameBoard *enemy );

	/**
	 * Places randomly ships on the ai board.
	*/
	void PlaceShips ();

	/**
	 * The AI make his turn.
	*/
	void Turn ();
};

#endif
