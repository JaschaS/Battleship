#ifndef MARKER_H
#define MARKER_H

/*	FIT2049 - Assignment 1
*	Cursor.h
*	Created by Jascha Scholz - 2015 - Monash Univeristy
*	A simple marker which indecates if something was hit or not.
*/

#include "StaticObject.h"
#include "Mesh.h"
#include "DirectXTK\SimpleMath.h"

class Marker : public StaticObject
{
private:
	bool m_visible{ false };
public:
	Marker() : StaticObject() {}
	Marker(Mesh* mesh, DirectX::SimpleMath::Vector3 position) : StaticObject(mesh, position) {}

	bool IsVisible() { return m_visible; };

	void SetVisible(bool visible) { m_visible = visible; }
};

#endif
