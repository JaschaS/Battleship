#include "Cursor.h"

/*	FIT2049 - Assignment 1
*	Cursor.cpp
*	Created by Jascha Scholz - 2015 - Monash Univeristy
*	Implementation of Cursor.h
*/

Cursor::Cursor () {}

Cursor::~Cursor ()
{
	m_cursorMesh.Release ();
	m_cursorMeshNotValid.Release ();
}

bool Cursor::Initialise (
	Direct3D *renderer,
	Shader &meshShader,
	DirectX::SimpleMath::Vector3 startPosition
	)
{
	if ( renderer == nullptr ) return false;

	bool error;

	error = m_cursorMesh.Initialise (
		renderer,
		"Assets/Meshes/SelectTile.obj",
		Color ( 1, 1, 1, 1 ),
		nullptr,
		&meshShader
	);
	if ( !error ) return false;

	error = m_cursorMeshNotValid.Initialise (
		renderer,
		"Assets/Meshes/SelectTile.obj",
		Color ( 0.2f, 0.2f, 0.2f, 1 ),
		nullptr,
		&meshShader
	);
	if ( !error ) return false;

	m_cursor = StaticObject ( &m_cursorMeshNotValid, startPosition );

	return true;
}

void Cursor::Render ( Direct3D *renderer, Camera &currentCamera )
{
	if ( renderer != nullptr && m_isVisible )
	{
		m_cursor.Render ( renderer, &currentCamera );
	}
}

void Cursor::Valid ( bool isValid )
{
	//Change if not equal
	if ( isValid != m_isValid )
	{
		//Change mesh.
		switch ( isValid )
		{
		case TRUE:
			m_cursor.SetMesh ( m_cursorMesh );
			break;
		case FALSE:
			m_cursor.SetMesh ( m_cursorMeshNotValid );
			break;
		default:
			break;
		}

		m_isValid = isValid;
	}
}

void  Cursor::Reset ( DirectX::SimpleMath::Vector3 position )
{
	m_isVisible = false;
	m_isValid = false;
	m_cursor.SetPosition ( position );
}