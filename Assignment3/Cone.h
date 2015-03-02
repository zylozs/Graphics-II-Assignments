/*
*	Vincent Loignon and Tyler Cifelli
*	Assignment 3
*	Cone.h
*	I certify that this assignment is entirely our own work.
*
*	Heavy influences from Frank Luna's code
*/

#ifndef _CONE_H
#define _CONE_H

#include "Cylinder.h"

class Cone : public Cylinder
{
protected:
	virtual void generateUVs();

public:
	Cone(float radius, float height, UINT sideFacetsNum);
	~Cone();

	virtual void Create(IDirect3DDevice9* gd3dDevice);
	virtual void Update(float dt);
	virtual void Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, D3DXVECTOR3& lightVec, D3DXVECTOR3& viewPos);
};

#endif