#pragma once
#include "DirectXTK\SimpleMath.h"
#include "Direct3D.h"
#include "Mesh.h"
#include "Camera.h"

using namespace DirectX::SimpleMath;

class GameObject
{
protected:
		Vector3 m_position;
		float m_rotX;
		float m_rotY;
		float m_rotZ;
		float m_scaleX;
		float m_scaleY;
		float m_scaleZ;

		Mesh* m_mesh;

public:
	GameObject();
	GameObject(Mesh *mesh);
	GameObject(Mesh *mesh, Vector3 position);
	GameObject(Mesh* mesh, Vector3 position, float rotX, float rotY, float rotZ);
	GameObject(Mesh* mesh, Vector3 position, float rotX, float rotY, float rotZ, float scaleX, float scaleY, float scaleZ);
	virtual ~GameObject();

	Vector3 GetPosition() { return m_position; }
	float GetXRotation() { return m_rotX; }
	float GetYRotation() { return m_rotY; }
	float GetZRotation() { return m_rotZ; }
	float GetXScale() { return m_scaleX; }
	float GetYScale() { return m_scaleY; }
	float GetZScale() { return m_scaleZ; }

	void SetPosition(Vector3 position) { m_position = position; }
	void SetXRotation(float x) { m_rotX = x; }
	void SetYRotation(float y) { m_rotY = y; }
	void SetZRotation(float z) { m_rotZ = z; }
	void SetXScale(float x) { m_scaleX = x; }
	void SetYScale(float y) { m_scaleY = y; }
	void SetZScale(float z) { m_scaleZ = z; }
	void SetMesh(Mesh& mesh) { m_mesh = &mesh; }


	virtual void Update(float timestep) = 0;
	virtual void Render(Direct3D* direct3D, Camera* currentCamera);

};