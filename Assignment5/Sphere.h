/*
*	Vincent Loignon and Tyler Cifelli
*	Assignment 4
*	Sphere.h
*	I certify that this assignment is entirely our own work.
*
*	Heavy influences from Frank Luna's code
*/

#ifndef _SPHERE_H
#define _SPHERE_H

#include "3DClasses/BaseObject3D.h"

class Sphere : public BaseObject3D
{
protected:
	FLOAT m_Radius;
	UINT m_SideFacetsNum;

	//virtual void calculateVertexBuffer(std::vector<Vertex>& vertices);
	//virtual void calculateIndexBuffer(std::vector<WORD>& indices);
	// 
	virtual void generateUVs();

public:
	Sphere(FLOAT radius, UINT sideFacetsNum);
	~Sphere();

	virtual void Create(IDirect3DDevice9* gd3dDevice);
	virtual void Update(float dt);
	virtual void Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, D3DXVECTOR3& lightVec, D3DXVECTOR3& viewPos);
};

#endif