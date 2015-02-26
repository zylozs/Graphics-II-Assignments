#include "Cone.h"
#include "3DClasses\Vertex.h"

Cone::Cone(float radius, float height, UINT sideFacetsNum)
	: Cylinder(radius, height, sideFacetsNum)
{
	//Second radius is zero because cone
	m_Radius2 = 0;
}

Cone::~Cone()
{

}

void Cone::Create(IDirect3DDevice9* gd3dDevice)
{
	// We aren't changing anything, so just use the base create function
	//BaseObject3D::Create(gd3dDevice);
	D3DXCreateCylinder(gd3dDevice, 0, m_Radius2, m_Height, m_SideFacetsNum, m_StackHeight, &m_Mesh, mp_Buffer);
}

void Cone::Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection)
{
	// We aren't changing anything, so just use the base render function
	BaseObject3D::Render(gd3dDevice, view, projection);
}

void Cone::Update(float dt)
{
	// We aren't changing anything, so just use the base update function
	BaseObject3D::Update(dt);
}

/*
void Cone::calculateVertexBuffer(std::vector<Vertex>& vertices)
{
	float theta = 2.0f * D3DX_PI / m_SideFacetsNum;

	//build bottom cap with top center index
	float y = 0.5f * m_Height;

	//Push top vertex
	vertices.push_back(Vertex(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f));

	//cache center
	m_CenterIndex1 = (UINT)vertices.size() - 1;

	//cache base
	m_BaseIndex2 = (UINT)vertices.size();

	//invert y
	y *= -1.0f;

	//Bottom cap
	for (UINT i = 0; i < m_SideFacetsNum; i++)
	{
		float x = m_Radius1 * cosf(i * theta);
		float z = m_Radius1 * sinf(i * theta);

		float u = x / m_Height + 0.5f;
		float v = z / m_Height + 0.5f;

		vertices.push_back(Vertex(x, y, z, 0.0f, -1.0f, 0.0f, u, v));
	}

	//cap center for bottom vertex
	vertices.push_back(Vertex(0.0f, y, 0.0f, 0.0f, -1.0f, 0.0f, 0.5f, 0.5f));

	//cache second center
	m_CenterIndex2 = (UINT)vertices.size() - 1;

}

void Cone::calculateIndexBuffer(std::vector<WORD>& indices)
{
	// Pointy part
	for (UINT i = 0; i < m_SideFacetsNum - 1; i++)
	{
		indices.push_back(m_CenterIndex1);
		indices.push_back(m_BaseIndex2 + i + 1);
		indices.push_back(m_BaseIndex2 + i);

		if (i == m_SideFacetsNum - 2)
		{
			indices.push_back(m_CenterIndex1);
			indices.push_back(m_BaseIndex2);
			indices.push_back(m_CenterIndex2 - 1);
		}
	}

	//Bottom Cap
	for (UINT i = 0; i < m_SideFacetsNum; i++)
	{
		indices.push_back(m_CenterIndex2);
		indices.push_back(m_BaseIndex2 + i);
		indices.push_back(m_BaseIndex2 + i + 1);

		if (i == m_SideFacetsNum - 1)
		{
			// Draw the last line
			indices.push_back(m_CenterIndex2);
			indices.push_back(m_CenterIndex2 - 1);
			indices.push_back(m_BaseIndex2);
		}
	}
}
*/