#ifndef SHIP_H
#define SHIP_H

/*	FIT2049 - Assignment 1
*	Ship.h
*	Created by Jascha Scholz - 2015 - Monash Univeristy
*	A ship which the player can place on his board.
*/

#include "Mesh.h"
#include "StaticObject.h"
#include "DirectXTK\SimpleMath.h"
#include "Direct3D.h"
#include "Camera.h"

enum Direction
{
	HORIZONTAL,
	VERTICAL
};

class Ship
{
private:
	Mesh m_shipMesh;
	Mesh m_shipMeshNotValid;
	StaticObject m_ship;

	Direction m_currentDirection{ VERTICAL };

	int m_shipLength;
	int m_id;
	void TurnShip(Direction direction);

	bool m_isVisible{ true };
	bool m_isPlaced{ false };
	bool m_isValid{ false };
public:
	Ship(int id);
	~Ship();

	bool Initialise(
		Direct3D *renderer,
		Shader &meshShader,
		DirectX::SimpleMath::Vector3 startPosition,
		const char* filename,
		int length
	);
	void Render(Direct3D *renderer, Camera &currentCamera);
	void Update(float timeStep) { m_ship.Update(timeStep); }

	/**
	 * Changes the direction (rotation) of the ship.
	*/
	void Flip() { TurnShip( (m_currentDirection == HORIZONTAL ? VERTICAL : HORIZONTAL ) ); }
	void Visible(bool isVisible) { m_isVisible = isVisible; }
	void Valid ( bool isValid );
	void Placed(bool isPlaced) { m_isPlaced = isPlaced; }
	void SetPosition(DirectX::SimpleMath::Vector3 position);
	void Reset ();

	Direction GetDirection () { return m_currentDirection; }
	bool IsValid() { return m_isValid; }
	bool IsPlaced() { return m_isPlaced; }
	bool IsVisible() { return m_isVisible; }
	int GetShipLength() { return m_shipLength; }
	int GetID() { return m_id; }
	DirectX::SimpleMath::Vector3 GetPosition() { return m_ship.GetPosition(); }
};


#endif
