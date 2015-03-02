/*
*	Vincent Loignon and Tyler Cifelli
*	Assignment 3
*	Cylinder.h
*	I certify that this assignment is entirely our own work.
*
*	Heavy influences from Frank Luna's code
*/

#ifndef _CYLINDER_H
#define _CYLINDER_H

#include "3DClasses\BaseObject3D.h"

class Cylinder : public BaseObject3D
{
protected:
	float m_Radius1;
	float m_Radius2;
	float m_Height;
	UINT m_SideFacetsNum;
	UINT m_StackHeight;
	float m_RadiusStep;

	UINT m_BaseIndex1;
	UINT m_CenterIndex1;
	UINT m_BaseIndex2;
	UINT m_CenterIndex2;

	//virtual void calculateVertexBuffer(std::vector<Vertex>& vertices);
	//virtual void calculateIndexBuffer(std::vector<WORD>& indices);
	virtual void generateUVs();

public:
	Cylinder(float radius, UINT height, UINT sideFacetsNum);
	~Cylinder();

	virtual void Create(IDirect3DDevice9* gd3dDevice);
	virtual void Update(float dt);
	virtual void Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, D3DXVECTOR3& lightVec, D3DXVECTOR3& viewPos);
};

#endif