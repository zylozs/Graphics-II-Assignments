#ifndef _SPHERE_H
#define _SPHERE_H

#include "3DClasses/BaseObject3D.h"

#define PI 3.14159265358979323846

class Sphere : public BaseObject3D
{
protected:
	float m_Radius;
	int m_SideFacetsNum;

	virtual void calculateVertexBuffer(std::vector<VertexPos>& vertices);
	virtual void calculateIndexBuffer(std::vector<WORD>& indices);

public:
	Sphere(float radius, int sideFacetsNum);
	~Sphere();

	virtual void Create(IDirect3DDevice9* gd3dDevice);
	virtual void Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection);
	virtual void Update(float dt);
};

#endif