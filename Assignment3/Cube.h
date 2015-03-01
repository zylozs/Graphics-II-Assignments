/*
*	Vince Loignon
*	Assignment 2
*	2/1/15
*	Sphere.h
*	I certify that this assignment is entirely our own work
* Heavy influences from Frank Luna's DX11 code
*/

#ifndef _CUBE_H
#define _CUBE_H

#include "3DClasses/BaseObject3D.h"

class Cube : public BaseObject3D
{
protected:
	float m_Length;
	float m_Width;
	float m_Height;

	virtual void generateUVs();

public:
	Cube(float length, float width, float height);
	~Cube();

	virtual void Create(IDirect3DDevice9* gd3dDevice);
	virtual void Update(float dt);
};

#endif