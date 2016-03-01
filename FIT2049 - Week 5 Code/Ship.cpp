#include "Ship.h"
#include "DirectXTK\SimpleMath.h"

/*	FIT2049 - Assignment 1
*	Ship.cpp
*	Created by Jascha Scholz - 2015 - Monash Univeristy
*	Implementation of Ship.h
*/

Ship::Ship(int id) : m_id(id)
{

}

Ship::~Ship()
{
	m_shipMesh.Release();
	m_shipMeshNotValid.Release ();
}

void Ship::TurnShip(Direction direction)
{
	switch (direction)
	{
	case HORIZONTAL:
		if (m_currentDirection != HORIZONTAL)
		{
			//90 deg to Rad
			m_ship.SetYRotation(90.0f / 180.0f * DirectX::XM_PI);
			m_currentDirection = HORIZONTAL;
		}
		break;
	case VERTICAL:
		if (m_currentDirection != VERTICAL)
		{
			m_ship.SetYRotation(0);
			m_currentDirection = VERTICAL;
		}
		break;
	default:
		break;
	}
}

bool Ship::Initialise(
	Direct3D *renderer,
	Shader &meshShader,
	DirectX::SimpleMath::Vector3 startPosition,
	const char* filename,
	int length
	)
{
	if (renderer == nullptr) return false;

	m_shipLength = length;

	bool error;

	error = m_shipMesh.Initialise(
		renderer,
		filename,
		Color(0.5f, 0.5f, 0.5f, 1.0f),
		nullptr,
		&meshShader
		);
	if (!error) return false;

	error = m_shipMeshNotValid.Initialise (
		renderer,
		filename,
		Color ( 1, 0.2f, 0.2f, 1.0f ),
		nullptr,
		&meshShader
		);
	if ( !error ) return false;

	m_ship = StaticObject(&m_shipMeshNotValid, startPosition);
	
	return true;
}

void Ship::Render(Direct3D *renderer, Camera &currentCamera)
{
	if (renderer != nullptr && m_isVisible)
	{
		m_ship.Render(renderer, &currentCamera);
	}
}

void Ship::SetPosition(DirectX::SimpleMath::Vector3 position)
{
	m_ship.SetPosition(position);
}

void Ship::Valid ( bool isValid )
{
	//Change if not equal
	if ( isValid != m_isValid )
	{
		//Change mesh.
		switch ( isValid )
		{
		case TRUE:
			m_ship.SetMesh ( m_shipMesh );
			break;
		case FALSE:
			m_ship.SetMesh ( m_shipMeshNotValid );
			break;
		default:
			break;
		}

		m_isValid = isValid;
	}
}

void Ship::Reset ()
{
	m_isVisible = false;
	m_isPlaced = false;
	m_isValid = false;
	m_ship.SetYRotation ( 0 );
	m_currentDirection = VERTICAL;
}