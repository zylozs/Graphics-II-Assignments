#ifndef _SPHERE_H
#define _SPHERE_H

#include "3DClasses/BaseObject3D.h"

class Sphere : public BaseObject3D
{
protected:
	float m_Radius;
	int m_SideFacetsNum;

	virtual void buildVertexBuffer(IDirect3DDevice9* gd3dDevice);
	virtual void buildIndexBuffer(IDirect3DDevice9* gd3dDevice);

public:
	Sphere(float radius, int sideFacetsNum);
	~Sphere();

	virtual void Create(IDirect3DDevice9* gd3dDevice);
	virtual void Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection);
	virtual void Update(float dt);
};

#endif