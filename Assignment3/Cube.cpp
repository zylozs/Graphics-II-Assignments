#include "Cube.h"
#include "3DClasses/Vertex.h"

Cube::Cube(float length, float width, float height)
{
	m_Length = length;
	m_Width = width;
	m_Height = height;
}

Cube::~Cube()
{

}

void Cube::Create(IDirect3DDevice9* gd3dDevice)
{
	// We aren't changing anything, so just use the base create function
	//BaseObject3D::Create(gd3dDevice);
	D3DXCreateBox(gd3dDevice, m_Width, m_Height, m_Length, &m_Mesh, mp_Buffer);
}

void Cube::Update(float dt)
{
	// We aren't changing anything, so just use the base update function
	BaseObject3D::Update(dt);
}

