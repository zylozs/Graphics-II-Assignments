#include "Sphere.h"
#include "3DClasses/Vertex.h"

Sphere::Sphere(float radius, int sideFacetsNum)
{
	m_Radius = radius;
	m_SideFacetsNum = sideFacetsNum;
}

Sphere::~Sphere()
{
	
}

void Sphere::Create(IDirect3DDevice9* gd3dDevice)
{
	buildVertexBuffer(gd3dDevice);
	buildIndexBuffer(gd3dDevice);
}

void Sphere::Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection)
{

}

void Sphere::Update(float dt)
{

}

void Sphere::buildVertexBuffer(IDirect3DDevice9* gd3dDevice)
{

}

void Sphere::buildIndexBuffer(IDirect3DDevice9* gd3dDevice)
{

}