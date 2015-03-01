/*
*	Vince Loignon
*	Assignment 2
*	2/1/15
*	Sphere.h
*	I certify that this assignment is entirely our own work
* Heavy influences from Frank Luna's DX11 code
*/

#ifndef _SPHERE_H
#define _SPHERE_H

#include "3DClasses/BaseObject3D.h"

class Sphere : public BaseObject3D
{
protected:
	float m_Radius;
	UINT m_SideFacetsNum;

	//virtual void calculateVertexBuffer(std::vector<Vertex>& vertices);
	//virtual void calculateIndexBuffer(std::vector<WORD>& indices);

public:
	Sphere(float radius, int sideFacetsNum);
	~Sphere();

	virtual void Create(IDirect3DDevice9* gd3dDevice);
	virtual void Update(float dt);
};

#endif