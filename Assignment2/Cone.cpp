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
	BaseObject3D::Create(gd3dDevice);
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

void Cone::calculateVertexBuffer(std::vector<VertexPos>& vertices)
{
	float theta = 2.0f * PI / m_SideFacetsNum;

	//cache base
	m_BaseIndex1 = (UINT)vertices.size();

	//build bottom cap with top center index
	float y = 0.5f * m_Height;

	//Push top vertex
	vertices.push_back(VertexPos(0.0f, y, 0.0f));

	//cache center
	m_CenterIndex1 = (UINT)vertices.size() - 1;

	//cache second base
	m_BaseIndex2 = (UINT)vertices.size();

	//invert y
	y *= -1.0f;

	//Bottom cap
	for (UINT i = 0; i < m_SideFacetsNum; i++)
	{
		float x = m_Radius1 * cosf(i * theta);
		float z = m_Radius1 * sinf(i * theta);

		vertices.push_back(VertexPos(x, y, z));
	}

	//cap center for bottom vertex
	vertices.push_back(VertexPos(0.0f, y, 0.0f));

	//cache second center
	m_CenterIndex2 = (UINT)vertices.size() - 1;

}

void Cone::calculateIndexBuffer(std::vector<WORD>& indices)
{
	// Top Cap
	for (UINT i = 0; i < m_SideFacetsNum; i++)
	{
		indices.push_back(m_CenterIndex1);
		indices.push_back(m_BaseIndex1 + i + 1);
		indices.push_back(m_BaseIndex1 + i);

		if (i == m_SideFacetsNum - 1)
		{
			// Draw the last line
			indices.push_back(m_CenterIndex1);
			indices.push_back(m_BaseIndex1);
			indices.push_back(m_CenterIndex1 - 1);
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

	// Calculate indices for center
	for (UINT i = 0; i < m_SideFacetsNum; i++)
	{
		if (i == m_SideFacetsNum - 1)
		{
			// Draw the last tris
			indices.push_back(m_BaseIndex1 + i);
			indices.push_back(m_BaseIndex1);
			indices.push_back(m_BaseIndex2 + i);

			indices.push_back(m_BaseIndex1);
			indices.push_back(m_BaseIndex2);
			indices.push_back(m_BaseIndex2 + i);

			break;
		}

		indices.push_back(m_BaseIndex1 + i);
		indices.push_back(m_BaseIndex1 + i + 1);
		indices.push_back(m_BaseIndex2 + i);

		indices.push_back(m_BaseIndex1 + i + 1);
		indices.push_back(m_BaseIndex2 + i + 1);
		indices.push_back(m_BaseIndex2 + i);
	}
}