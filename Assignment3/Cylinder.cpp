#include "Cylinder.h"
#include "3DClasses\Vertex.h"

Cylinder::Cylinder(float radius, UINT height, UINT sideFacetsNum)
	: m_Radius1(radius), m_Radius2(radius),
	m_Height(height), m_SideFacetsNum(sideFacetsNum)
{
	m_StackHeight = (UINT)m_Height / m_SideFacetsNum;

	//calculate radius step per stack level
	m_RadiusStep = (m_Radius1 - m_Radius2) / m_SideFacetsNum;
	
}

Cylinder::~Cylinder()
{

}

void Cylinder::Create(IDirect3DDevice9* gd3dDevice)
{
	// We aren't changing anything, so just use the base create function
	//BaseObject3D::Create(gd3dDevice);
	D3DXCreateCylinder(gd3dDevice, m_Radius1, m_Radius2, m_Height, m_SideFacetsNum, m_SideFacetsNum, &m_Mesh, mp_Buffer);
}

void Cylinder::Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection)
{
	// We aren't changing anything, so just use the base render function
	BaseObject3D::Render(gd3dDevice, view, projection);
}

void Cylinder::Update(float dt)
{
	// We aren't changing anything, so just use the base update function
	BaseObject3D::Update(dt);
}

// A large chunk of this function was taken and adapted from Frank Luna's DirectX 11 Shapes Demo
// Credit goes to him for most of this.
/*
void Cylinder::calculateVertexBuffer(std::vector<Vertex>& vertices)
{
	float theta = 2.0f * D3DX_PI / m_SideFacetsNum;

	//cache base
	m_BaseIndex1 = (UINT)vertices.size();
	
	//Build top and bottom caps
	float y = 0.5f * m_Height;

 	// Top Cap
	for (UINT i = 0; i < m_SideFacetsNum; i++)
	{
		float x = m_Radius1 * cosf(i * theta);
		float z = m_Radius1 * sinf(i * theta);

		float u = x / m_Height + 0.5f;
		float v = z / m_Height + 0.5f;

		vertices.push_back(Vertex(x, y, z, 0.0f, 1.0f, 0.0f, u, v));
	}

	// cap center top vertex
	vertices.push_back(Vertex(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f));

	//cache center
	m_CenterIndex1 = (UINT)vertices.size() - 1;

	//cache second base
	m_BaseIndex2 = (UINT)vertices.size();

	// invert y
	y *= -1.0f;

	// Bottom Cap
	for (UINT i = 0; i < m_SideFacetsNum; i++)
	{
		float x = m_Radius2 * cosf(i * theta);
		float z = m_Radius2 * sinf(i * theta);

		float u = x / m_Height + 0.5f;
		float v = z / m_Height + 0.5f;

		vertices.push_back(Vertex(x, y, z, 0.0f, -1.0f, 0.0f, u, v));
	}

	//cap center bottom vertex
	vertices.push_back(Vertex(0.0f, y, 0.0f, 0.0f, -1.0f, 0.0f, 0.5f, 0.5f));

	// cache second center
	m_CenterIndex2 = (UINT)vertices.size() - 1;
}

// A large chunk of this function was taken and adapted from Frank Luna's DirectX 11 Shapes Demo
// Credit goes to him for most of this.
void Cylinder::calculateIndexBuffer(std::vector<WORD>& indices)
{
	//create indicies for caps

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

	// Bottom Cap
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
*/