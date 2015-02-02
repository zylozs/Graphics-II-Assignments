#include "Cylinder.h"
#include "3DClasses\Vertex.h"

Cylinder::Cylinder(float radius1, float radius2, float length)
	: m_Radius1(radius1), m_Radius2(radius2), m_Length(length)
{
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
	// Create our vertex list before locking the buffer

	float phiStep = (float)PI / m_SideFacetsNum;
	float thetaStep = 2.0f * (float)PI / m_SideFacetsNum;

	// Add our Top Pole
	vertices.push_back(VertexPos(0.0f, m_Radius1, 0.0f));

	// Compute vertices for each stack ring (do not count the poles as rings)
	for (int i = 1; i <= m_SideFacetsNum - 1; i++)
	{
		float phi = i * phiStep;

		// Vertices of ring
		for (int j = 0; j <= m_SideFacetsNum; j++)
		{
			float theta = j * thetaStep;

			VertexPos v;

			// spherical to cartesian
			v.pos.x = m_Radius * sinf(phi) * cosf(theta);
			v.pos.y = m_Radius * cosf(phi);
			v.pos.z = m_Radius * sinf(phi) * sinf(theta);

			vertices.push_back(v);
		}
	}

	// Add our Bottom Pole
	vertices.push_back(VertexPos(0.0f, m_Radius2, 0.0f));
}

// A large chunk of this function was taken and adapted from Frank Luna's DirectX 11 Shapes Demo
// Credit goes to him for most of this.
void Cylinder::calculateIndexBuffer(std::vector<WORD>& indices)
{
	// Calculate the Indices before writing to the buffer

	// Add our Top Pole
	for (int i = 1; i < m_Length; i++)
	{
		indices.push_back(0);
		indices.push_back(i + 1);
		indices.push_back(i);
	}

	// Compute Indices for the rings

	// Offset the indices to the index of the first vertex in the first ring
	// This is just skipping the top pole vertex
	UINT baseIndex = 1;
	UINT ringVertexCount = m_SideFacetsNum + 1;
	for (int i = 0; i < m_SideFacetsNum - 2; i++)
	{
		for (int j = 0; j < m_SideFacetsNum; ++j)
		{
			indices.push_back(baseIndex + i * ringVertexCount + j);
			indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
			indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
		}
	}

	// Add our Bottom Pole

	// South pole vertex was added last
	UINT southPoleIndex = (UINT)m_Vertices - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for (int i = 0; i < m_SideFacetsNum; i++)
	{
		indices.push_back(southPoleIndex);
		indices.push_back(baseIndex + i);
		indices.push_back(baseIndex + i + 1);
	}
}