//=============================================================================
// Camera.h by Frank Luna (C) 2004 All Rights Reserved.
//=============================================================================

#ifndef CAMERA_H
#define CAMERA_H

#include <d3dx9.h>
#include "d3dUtil.h"
#include "3DClasses/BaseObject3D.h"

class Camera : public BaseObject3D
{
public:
	Camera();
	~Camera();

	const D3DXMATRIX& getView() const { return m_View; }
	const D3DXMATRIX& getProj() const { return m_Proj; }
	const D3DXMATRIX& getViewProj() const { return m_ViewProj; }

	const D3DXVECTOR3& getRight() const { return m_RightW; }
	const D3DXVECTOR3& getUp() const { return m_UpW; }
	const D3DXVECTOR3& getLook() const { return m_LookW; }

	void lookAt(D3DXVECTOR3& pos, D3DXVECTOR3& target, D3DXVECTOR3& up);
	void setLens(float fov, float aspect, float nearZ, float farZ);
	void setSpeed(float s) { m_Speed = s; }

	// Box coordinates should be relative to world space.
	//bool isVisible(const AABB& box)const;

	virtual void Update(float dt);

	virtual void dispose();
	virtual void Create(IDirect3DDevice9* gd3dDevice);

protected:
	void buildView();
	void buildWorldFrustumPlanes();

protected:
	D3DXMATRIX m_View;
	D3DXMATRIX m_Proj;
	D3DXMATRIX m_ViewProj;

	// Relative to world space.
	D3DXVECTOR3 m_PosW;
	D3DXVECTOR3 m_RightW;
	D3DXVECTOR3 m_UpW;
	D3DXVECTOR3 m_LookW;

	float m_Speed;

	// Frustum Planes
	D3DXPLANE m_FrustumPlanes[6]; // [0] = near
	                              // [1] = far
	                              // [2] = left
	                              // [3] = right
	                              // [4] = top
	                              // [5] = bottom
};

extern Camera* g_Camera;

#endif // CAMERA_H