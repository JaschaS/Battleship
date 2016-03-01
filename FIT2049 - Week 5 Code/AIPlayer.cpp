#include "AIPlayer.h"
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <Windows.h>
#include "Ship.h"

/*	FIT2049 - Assignment 1
*	AIPlayer.cpp
*	Created by Jascha Scholz - 2015 - Monash Univeristy
*	Implementation of AIPlayer.h
*/

AIPlayer::AIPlayer ()
{
}

bool AIPlayer::Initialise ( GameBoard *board, GameBoard *enemy )
{
	if ( board == nullptr || enemy == nullptr ) return false;

	m_board = board;
	m_enemyBoard = enemy;

	PlaceShips ();

	return true;
}

void AIPlayer::PlaceShips ()
{

	if ( m_board == nullptr ) return;

	int shipSize[] = {5, 4, 3, 3, 2};
	int shipIds[] = {1, 2, 3, 4, 5};
	int valuesCheck[2];
	int random;
	int startX;
	int startY;
	Direction shipDirection;
	std::vector<BoardPositions> positions;

	//seed for random to generate a different value every time the program is run
	srand ( time ( NULL ) );

	

	positions = m_board->GetBoardPositions ();

	valuesCheck[0] = FREE;

	for ( int i = 0; i < 5; )
	{
		//Number between 0 and position.size - 1
		//For the start position.
		random = rand () % positions.size();

		startX = positions[random].x;
		startY = positions[random].y;

		//For the ship direction.
		random = rand () % 2;
		switch ( random )
		{
		case 0:
			shipDirection = HORIZONTAL;
			break;
		case 1:
			shipDirection = VERTICAL;
			break;
		default:
			shipDirection = HORIZONTAL;
			break;
		}

		valuesCheck[1] = shipIds[i];

		//Try to add the ship.
		if ( m_board->AddValueToPosition ( startX, startY, shipDirection, shipSize[i], valuesCheck, shipIds[i], 1 ) )
		{
			++i;
		}
	}

}

void AIPlayer::Turn ()
{
	int random;
	int x;
	int z;
	std::vector<BoardPositions> positions;
	bool shot = { false };

	Sleep ( 1000 );

	//seed for random to generate a different value every time the program is run
	srand ( time ( NULL ) );

	positions = m_enemyBoard->GetBoardPositions ();

	do
	{
		//Number between 0 and position.size - 1
		//For the start position.
		random = rand () % positions.size ();

		x = positions[random].x;
		z = positions[random].y;

		shot = m_enemyBoard->SetMarker ( x, z );

	} while ( !shot );

}