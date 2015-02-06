#include "Cylinder.h"
#include "3DClasses\Vertex.h"

Cylinder::Cylinder(float radius, float height, UINT sideFacetsNum)
	: m_Radius1(radius), m_Radius2(radius),
	m_Height(height), m_SideFacetsNum(sideFacetsNum)
{
	m_StackHeight = m_Height / m_SideFacetsNum;

	//calculate radius step per stack level
	m_RadiusStep = (m_Radius1 - m_Radius2) / m_SideFacetsNum;
	
}

Cylinder::~Cylinder()
{

}

void Cylinder::Create(IDirect3DDevice9* gd3dDevice)
{
	// We aren't changing anything, so just use the base create function
	BaseObject3D::Create(gd3dDevice);
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
void Cylinder::calculateVertexBuffer(std::vector<VertexPos>& vertices)
{
	float theta = 2.0f * PI / m_SideFacetsNum;

	// Create the rings
	for (UINT i = 0; i < m_SideFacetsNum + 1; i++)
	{

		//calculate radius and height values
		float y = -0.5f * m_Height + i * m_StackHeight;
		float r = m_Radius2 + i * m_RadiusStep;

		//create vertices of ring
		for (UINT j = 0; j < m_SideFacetsNum; j++)
		{
			VertexPos vertex;

			//Sin and Cos values
			float c = cosf(j * theta);
			float s = sinf(j * theta);

			//place vertex position
			vertex.pos.x = r * c;
			vertex.pos.y = y;
			vertex.pos.z = r * s;

			vertices.push_back(vertex);
		}
	}

	//cache base
	m_BaseIndex1 = (UINT)vertices.size();
	
	//Build top and bottom caps
	float y = 0.5f * m_Height;

 	// Top Cap
	for (UINT i = 0; i < m_SideFacetsNum; i++)
	{
		float x = m_Radius1 * cosf(i * theta);
		float z = m_Radius1 * sinf(i * theta);

		vertices.push_back(VertexPos(x, y, z));
	}

	// cap center top vertex
	vertices.push_back(VertexPos(0.0f, y, 0.0f));

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

		vertices.push_back(VertexPos(x, y, z));
	}

	//cap center bottom vertex
	vertices.push_back(VertexPos(0.0f, y, 0.0f));

	// cache second center
	m_CenterIndex2 = (UINT)vertices.size() - 1;
}

// A large chunk of this function was taken and adapted from Frank Luna's DirectX 11 Shapes Demo
// Credit goes to him for most of this.
void Cylinder::calculateIndexBuffer(std::vector<WORD>& indices)
{
	//calculate vertex count for ring, it has one more  because of different coordinates
	UINT ringVertexCount = m_SideFacetsNum + 1;

	//generate indicies
	for (UINT i = 0; i < m_SideFacetsNum; i++)
	{
		for (UINT j = 0; j < m_SideFacetsNum; j++)
		{
			indices.push_back(i * ringVertexCount + j);
			indices.push_back((i + 1) * ringVertexCount + j);
			indices.push_back((i + 1) * ringVertexCount + (j + 1));

			indices.push_back(i * ringVertexCount + j);
			indices.push_back((i + 1) * ringVertexCount + (j + 1));
			indices.push_back(i * ringVertexCount + (j + 1));
		}
	}

	//create indicies for caps

	// Top Cap
	for (UINT i = 0; i < m_SideFacetsNum; i++)
	{
		indices.push_back(m_CenterIndex1);
		indices.push_back(m_BaseIndex1 + i + 1);
		indices.push_back(m_BaseIndex1 + i);
	}

	// Bottom Cap
	for (UINT i = 0; i < m_SideFacetsNum; i++)
	{
		indices.push_back(m_CenterIndex2);
		indices.push_back(m_BaseIndex2 + i + 1);
		indices.push_back(m_BaseIndex2 + i);
	}
}