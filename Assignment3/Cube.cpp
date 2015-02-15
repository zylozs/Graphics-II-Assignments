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
	BaseObject3D::Create(gd3dDevice);
}

void Cube::Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection)
{
	// We aren't changing anything, so just use the base render function
	BaseObject3D::Render(gd3dDevice, view, projection);
}

void Cube::Update(float dt)
{
	// We aren't changing anything, so just use the base update function
	BaseObject3D::Update(dt);
}

void Cube::calculateVertexBuffer(std::vector<Vertex>& vertices)
{
	float l = 0.5f * m_Length;
	float w = 0.5f * m_Width;
	float h = 0.5f * m_Height;

	vertices.push_back(Vertex(-l, -w, -h, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f));
	vertices.push_back(Vertex(-l, -w, +h, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f));
	vertices.push_back(Vertex(-l, +w, +h, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f));
	vertices.push_back(Vertex(-l, +w, -h, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f));

	vertices.push_back(Vertex(+l, -w, -h, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f));
	vertices.push_back(Vertex(+l, +w, -h, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f));
	vertices.push_back(Vertex(+l, +w, +h, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f));
	vertices.push_back(Vertex(+l, -w, +h, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f));

	vertices.push_back(Vertex(-l, -w, +h, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f));
	vertices.push_back(Vertex(+l, -w, +h, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f));
	vertices.push_back(Vertex(+l, +w, +h, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f));
	vertices.push_back(Vertex(-l, +w, +h, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f));

	vertices.push_back(Vertex(-l, -w, -h, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f));
	vertices.push_back(Vertex(-l, +w, -h, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f));
	vertices.push_back(Vertex(+l, +w, -h, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f));
	vertices.push_back(Vertex(+l, -w, -h, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f));

	vertices.push_back(Vertex(+l, -w, -h, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f));
	vertices.push_back(Vertex(+l, -w, +h, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f));
	vertices.push_back(Vertex(-l, -w, +h, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f));
	vertices.push_back(Vertex(-l, -w, -h, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f));

	vertices.push_back(Vertex(-l, +w, -h, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f));
	vertices.push_back(Vertex(-l, +w, +h, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f));
	vertices.push_back(Vertex(+l, +w, +h, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f));
	vertices.push_back(Vertex(+l, +w, -h, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f));
}

void Cube::calculateIndexBuffer(std::vector<WORD>& indices)
{
	// Front face.
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);

	// Back face.
	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(4);
	indices.push_back(6);
	indices.push_back(7);

	// Left face.
	indices.push_back(8);
	indices.push_back(9);
	indices.push_back(10);
	indices.push_back(8);
	indices.push_back(10);
	indices.push_back(11);

	// Right face.
	indices.push_back(12);
	indices.push_back(13);
	indices.push_back(14);
	indices.push_back(12);
	indices.push_back(14);
	indices.push_back(15);

	// Top face.
	indices.push_back(16);
	indices.push_back(17);
	indices.push_back(18);
	indices.push_back(16);
	indices.push_back(18);
	indices.push_back(19);

	// Bottom face.
	indices.push_back(20);
	indices.push_back(21);
	indices.push_back(22);
	indices.push_back(20);
	indices.push_back(22);
	indices.push_back(23);
}