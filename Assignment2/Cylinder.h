/*
*	Tyler Cifelli + Vince Loignon
*	Assignment 2
*	2/1/15
*	Cylinder.h
*	I certify that this assignment is entirely my own work
* Heavy influences from Frank Luna's DX11 code
*/

#ifndef _CYLINDER_H
#define _CYLINDER_H

#include "3DClasses\BaseObject3D.h"
typedef unsigned int uint;

class Cylinder : public BaseObject3D
{
protected:
	float m_Radius1;
	float m_Radius2;
	float m_Height;
	uint m_SliceCount;
	uint m_StackCount;
	float m_StackHeight;
	float m_RadiusStep;
	uint m_BaseIndex;
	uint m_CenterIndex;

	virtual void calculateVertexBuffer(std::vector<VertexPos>& vertices);
	virtual void calculateIndexBuffer(std::vector<WORD>& indices);

public:
	Cylinder(float radius1, float radius2, float height, uint slices, uint stacks);
	~Cylinder();

	virtual void Create(IDirect3DDevice9* gd3dDevice);
	virtual void Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection);
	virtual void Update(float dt);
};

#endif