#ifndef GAME_BOARD_H
#define GAME_BOARD_H

/*	FIT2049 - Assignment 1
*	GameBoard.h
*	Created by Jascha Scholz - 2015 - Monash Univeristy
*	A gameboard which contains ship positions and markers.
*	Also checks if positions are valid.
*/

#include "StaticObject.h"
#include "Mesh.h"
#include "Marker.h"
#include "DirectXTK\SimpleMath.h"
#include "Direct3D.h"
#include "Camera.h"
#include "Ship.h"

#include <vector>

#define FREE 0
#define BOMBED -1

enum MarkerTypes
{
	HIT_MARKER = 0,
	WATER_MARKER
};

struct BoardPositions
{
	int x;
	int y;
	int value;
};

class GameBoard
{
private:
	DirectX::SimpleMath::Vector3 boardPosition;

	Mesh m_boardMesh;
	Mesh m_textPlaneMesh;
	Mesh m_hitMarkerMesh;
	Mesh m_waterMarkerMesh;

	StaticObject m_board;
	StaticObject m_textPlane;
	std::vector<Marker> m_hitMarkers;		//A board can have 17 hit markers.
	std::vector<Marker> m_waterMarkers;	//A board can have 83 water markers.

	std::vector<BoardPositions> m_boardMap;

	int m_aircraftCarrier{ 5 };
	int m_battleship{ 4 };
	int m_destroyer{ 3 };
	int m_submarine{ 3 };
	int m_patrolBoat{ 2 };

	int m_hits{ 0 };
	int m_missed{ 0 };

	//Returns the next marker which is not visible currently.
	Marker* GetNextFreeMarker(std::vector<Marker> &markers);

	void RenderMarkerInVector(std::vector<Marker> &markers, Direct3D *renderer, Camera &currentCamera);

	/**
	* Checks if multiple position are valid.
	* The positions are valid, if:
	* 1) The positions are inside the board.
	* 2) The positions down cross a ship.
	* 3) The value of the position is equals to the parameter value.
	*
	* If the vectors don't have the same size,
	* not all of the values of the longest one
	* will be used.
	*/
	bool ArePositionsValid ( std::vector<int> x, std::vector<int> y, int *values, int arraySize );

	/**
	 * Returns the BoardPosition to the given position.
	 * A nullptr will be returned, if the parameter position couldn't be found
	*/
	BoardPositions* GetPosition (int x, int y);

	/**
	 * Set the value of the game board.
	*/
	void SetBoardValues ( std::vector<int> &x, std::vector<int> &y, int value );

public:
	GameBoard();
	virtual ~GameBoard();

	bool Initialise(
		Direct3D *renderer,
		Shader &meshShader,
		DirectX::SimpleMath::Vector3 position,
		DirectX::SimpleMath::Color boardColor
	);
	void Update(float timestep);
	void Render(Direct3D *renderer, Camera &currentCamera);

	/**
	 * Try's to add the ship to the position;
	 * Returns false if it not possible. 
	 * Returns true if the ship was set to the position.
	*/
	bool AddValueToPosition (
		int startX,
		int startY,
		Direction shipDirection,
		int shipLength,
		int *checkValues,
		int addValue, 
		int arraySize
	);

	/**
	 * Set a marker to a specific position.
	*/
	bool SetMarker(int x, int y);

	/**
	 * Print outs the GameBoard on the console.
	*/
	void PrintBoard ();

	/**
	* Checks if the given position is valid.
	* The position is valid, if:
	* 1) The position is inside the board.
	* 2) There is no other ship on this position.
	* 3) The value of the position is equals to the parameter value.
	* 
	* Checks only if the position is valid, if values == nullptr or
	* the arraySize is <= 0.
	*/
	bool IsPositionValid ( int x, int y, int *values, int arraySize );

	std::vector<BoardPositions> GetBoardPositions () { return m_boardMap; }

	int ShipsLeft ();
	int Hits () { return m_hits; }
	int Missed () { return m_missed; }

	void Reset ();
};

#endif
