/*
*	Vincent Loignon and Tyler Cifelli
*	Assignment 3
*	Teapot.h
*	I certify that this assignment is entirely our own work.
*/

#ifndef _TEAPOT_H
#define _TEAPOT_H

#include "3DClasses/BaseObject3D.h"

class Teapot : public BaseObject3D
{
protected:

	virtual void generateUVs();
public:
	Teapot();
	~Teapot();

	virtual void Create(IDirect3DDevice9* gd3dDevice);
	virtual void Update(float dt);
	virtual void Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, D3DXVECTOR3& lightVec, D3DXVECTOR3& viewPos);
};

#endif