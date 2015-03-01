/*
*	Vince Loignon and Tyler Cifelli
*	Assignment 2
*	Cone.h
*	I certify that this assignment is entirely our own work.
*
*	Heavy influences from Frank Luna's DX11 code
*/

#ifndef _CONE_H
#define _CONE_H

#include "Cylinder.h"

class Cone : public Cylinder
{
protected:
	//virtual void calculateVertexBuffer(std::vector<Vertex>& vertices);
	//virtual void calculateIndexBuffer(std::vector<WORD>& indicies);
	virtual void generateUVs();

public:
	Cone(float radius, float height, UINT sideFacetsNum);
	~Cone();

	virtual void Create(IDirect3DDevice9* gd3dDevice);
	virtual void Update(float dt);
};

#endif