#ifndef ORTHOGONAL_CAMERA_H
#define ORTHOGONAL_CAMERA_H

/*	FIT2049 - Assignment 1
*	OrthogonalCamera.h
*	Created by Jascha Scholz - 2015 - Monash Univeristy
*	A orthogonal camera which will be used to render
*	everything.
*/

#include "Camera.h"

class OrthogonalCamera : public Camera
{
private:
	float m_width;
	float m_height;
public:
	//Width and height defines how big the viewport of the orthogonal camera is
	//Bigger means the objects get small. Choose a small value => objects get bigger.
	OrthogonalCamera(float width, float height) :
		Camera(width, height), m_width(width), m_height(height) {}

	virtual void Update(float timestep)
	{
		if (m_viewDirty)
		{
			m_view = Matrix::CreateLookAt(m_position, m_lookAtTarget, m_up);
			m_viewDirty = false;
		}

		if (m_projectionDirty)
		{
			m_projection = Matrix::CreateOrthographic(m_width, m_height, 0, 100);
			m_projectionDirty = false;
		}
	}
};

#endif