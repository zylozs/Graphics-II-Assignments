/*
*	Tyler Cifelli + Vince Loignon
*	Assignment 2
*	2/1/15
*	Cylinder.h
*	I certify that this assignment is entirely my own work
*/

#ifndef _CYLINDER_H
#define _CYLINDER_H

#include "3DClasses\BaseObject3D.h"

class Cylinder : public BaseObject3D
{
protected:
	float m_Radius1;
	float m_Radius2;
	float m_Length;
	int m_SideFacetsNum;

	virtual void calculateVertexBuffer(std::vector<VertexPos>& vertices);
	virtual void calculateIndexBuffer(std::vector<WORD>& indices);

public:
	Cylinder(float radius1, float radius2, float length, int numFacets);
	~Cylinder();

	virtual void Create(IDirect3DDevice9* gd3dDevice);
	virtual void Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection);
	virtual void Update(float dt);
};

#endif