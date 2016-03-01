#ifndef CURSOR_H
#define CURSOR_H

/*	FIT2049 - Assignment 1
*	Cursor.h
*	Created by Jascha Scholz - 2015 - Monash Univeristy
*	A cursor can be used by the player to set markers
*   on the board.
*/

#include "Mesh.h"
#include "StaticObject.h"

class Cursor
{
private:
	Mesh m_cursorMesh;
	Mesh m_cursorMeshNotValid;
	StaticObject m_cursor;

	bool m_isVisible{ true };
	bool m_isValid{ false };
public:
	Cursor ();
	~Cursor ();

	bool Initialise (
		Direct3D *renderer,
		Shader &meshShader,
		DirectX::SimpleMath::Vector3 startPosition
	);

	void Render ( Direct3D *renderer, Camera &currentCamera );
	void Valid ( bool isValid );
	void Update ( float timeStep ) { m_cursor.Update ( timeStep ); }
	void Visible ( bool isVisible ) { m_isVisible = isVisible; }
	void SetPosition ( DirectX::SimpleMath::Vector3 position ) { m_cursor.SetPosition ( position ); }
	void Reset ( DirectX::SimpleMath::Vector3 position );

	bool IsValid () { return m_isValid; }
	bool IsVisible () { return m_isVisible; }
	DirectX::SimpleMath::Vector3 GetPosition () { return m_cursor.GetPosition (); }
};

#endif
