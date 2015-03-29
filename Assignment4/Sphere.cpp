/*
*	Vincent Loignon and Tyler Cifelli
*	Assignment 3
*	Sphere.cpp
*	I certify that this assignment is entirely our own work.
*
*	Heavy influences from Frank Luna's code
*/

#include "Sphere.h"
#include "3DClasses/Vertex.h"

Sphere::Sphere(float radius, int sideFacetsNum)
{
	m_Radius = radius;
	m_SideFacetsNum = sideFacetsNum;
	m_Name = "Sphere";
}

Sphere::~Sphere()
{
	
}

void Sphere::Create(IDirect3DDevice9* gd3dDevice)
{
	D3DXCreateSphere(gd3dDevice, m_Radius, m_SideFacetsNum, m_SideFacetsNum, &m_Mesh, mp_Buffer);

	generateUVs();
	generateTBNs();
}

void Sphere::generateUVs()
{
	// Grab our vertex description
	D3DVERTEXELEMENT9 elements[64];
	UINT numElements = 0;
	Vertex::Decl->GetDeclaration(elements, &numElements);

	// Create a copy of our sphere mesh using our vertex description instead of the old one
	ID3DXMesh* temp = 0;
	HR(m_Mesh->CloneMesh(D3DXMESH_SYSTEMMEM, elements, gd3dDevice, &temp));

	// Release our old mesh since we have a copy we will be modifying in system memory
	ReleaseCOM(m_Mesh);

	// Lock the vertex buffer for modification
	Vertex* vertices = 0;
	HR(temp->LockVertexBuffer(0, (void**)&vertices));

	// Iterate through the vertices and calculate and store the uv coordinates
	for (UINT i = 0; i < temp->GetNumVertices(); i++)
	{
		D3DXVECTOR3 pos = vertices[i].Position;

		// Phi and theta give us our uvs, but they aren't in [0, 1] range
		float theta = atan2f(pos.z, pos.x);
		float phi = acosf(pos.y / sqrtf(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z));

		// Scale Phi and Theta to [0, 1] range
		float u = theta / (2.0f * D3DX_PI);
		float v = phi / D3DX_PI;

		// Save our uvs in the vertices
		vertices[i].TexCoord.x = u;
		vertices[i].TexCoord.y = v;
	}

	// Unlock the buffer since we no longer need it
	HR(temp->UnlockVertexBuffer());

	// Clone the copy of the mesh back into the member variable with hardware friendly tags
	HR(temp->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, elements, gd3dDevice, &m_Mesh));

	// Release our local copy since we no longer need it
	ReleaseCOM(temp);
}

void Sphere::Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, D3DXVECTOR3& lightVec, D3DXVECTOR3& viewPos)
{
	// Tell it to wrap the u coordinate
	HR(gd3dDevice->SetRenderState(D3DRS_WRAP0, D3DWRAP_U));

	BaseObject3D::Render(gd3dDevice, view, projection, lightVec, viewPos);

	// Set it back to what it was
	HR(gd3dDevice->SetRenderState(D3DRS_WRAP0, 0));
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
			v.TexCoord.x = theta / D3DX_PI;
			v.TexCoord.y = phi / D3DX_PI;

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
