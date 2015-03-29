/*
*	Vincent Loignon and Tyler Cifelli
*	Assignment 4
*	Torus.h
*	I certify that this assignment is entirely our own work.
*/

#ifndef _TORUS_H
#define _TORUS_H

#include "3DClasses/BaseObject3D.h"

class Torus : public BaseObject3D
{
protected:
	FLOAT m_InnerRadius;
	FLOAT m_OuterRadius;
	UINT m_NumFaces;

	virtual void generateUVs();

public:
	Torus(FLOAT innerRadius, FLOAT outerRadius, UINT numFaces);
	~Torus();

	virtual void Create(IDirect3DDevice9* gd3dDevice);
	virtual void Update(float dt);
	virtual void Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, D3DXVECTOR3& lightVec, D3DXVECTOR3& viewPos);
};

#endif