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
	// We aren't changing anything, so just use the base create function
	//BaseObject3D::Create(gd3dDevice);
	D3DXCreateSphere(gd3dDevice, m_Radius, m_SideFacetsNum, m_SideFacetsNum, &m_Mesh, mp_Buffer);
}

void Sphere::Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection)
{
	// We aren't changing anything, so just use the base render function
	BaseObject3D::Render(gd3dDevice, view, projection);
}

void Sphere::Update(float dt)
{
	// We aren't changing anything, so just use the base update function
	BaseObject3D::Update(dt);
}

// A large chunk of this function was taken and adapted from Frank Luna's DirectX 11 Shapes Demo
// Credit goes to him for most of this.
/*
void Sphere::calculateVertexBuffer(std::vector<Vertex>& vertices)
{
	// Create our vertex list before locking the buffer

	float phiStep = D3DX_PI / m_SideFacetsNum;
	float thetaStep = 2.0f * D3DX_PI / m_SideFacetsNum;

	// Add our Top Pole
	vertices.push_back(Vertex(0.0f, m_Radius, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f));

	// Compute vertices for each stack ring (do not count the poles as rings)
	for (int i = 1; i <= m_SideFacetsNum - 1; i++)
	{
		float phi = i * phiStep;

		// Vertices of ring
		for (int j = 0; j <= m_SideFacetsNum; j++)
		{
			float theta = j * thetaStep;

			Vertex v;

			// spherical to cartesian
			v.Position.x = m_Radius * sinf(phi) * cosf(theta);
			v.Position.y = m_Radius * cosf(phi);
			v.Position.z = m_Radius * sinf(phi) * sinf(theta);

			// Get our normal
			D3DXVec3Normalize(&v.Normal, &v.Position);

			// Get our texture coordinates (UV)
			v.TexC.x = theta / D3DX_PI;
			v.TexC.y = phi / D3DX_PI;

			vertices.push_back(v);
		}
	}

	// Add our Bottom Pole
	vertices.push_back(Vertex(0.0f, -m_Radius, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f));
}

// A large chunk of this function was taken and adapted from Frank Luna's DirectX 11 Shapes Demo
// Credit goes to him for most of this.
void Sphere::calculateIndexBuffer(std::vector<WORD>& indices)
{
	// Calculate the Indices before writing to the buffer

	// Add our Top Pole
	for (int i = 1; i < m_SideFacetsNum; i++)
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
*/