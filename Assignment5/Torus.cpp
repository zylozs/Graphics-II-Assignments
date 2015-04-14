/*
*	Vincent Loignon and Tyler Cifelli
*	Assignment 4
*	Torus.cpp
*	I certify that this assignment is entirely our own work.
*/

#include "Torus.h"
#include "3DClasses/Vertex.h"

Torus::Torus(FLOAT innerRadius, FLOAT outerRadius, UINT numFaces)
{
	m_InnerRadius = innerRadius;
	m_OuterRadius = outerRadius;
	m_NumFaces = numFaces;
	m_Name = "Torus";
}

Torus::~Torus()
{

}

void Torus::Create(IDirect3DDevice9* gd3dDevice)
{
	HR(D3DXCreateTorus(gd3dDevice, m_InnerRadius, m_OuterRadius, m_NumFaces, m_NumFaces, &m_Mesh, mp_Buffer));
	generateUVs();
	generateTBNs();
}

void Torus::generateUVs()
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
	HR(temp->CloneMesh(D3DXMESH_MANAGED, elements, gd3dDevice, &m_Mesh));

	// Release our local copy since we no longer need it
	ReleaseCOM(temp);
}

void Torus::Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, D3DXVECTOR3& lightVec, D3DXVECTOR3& viewPos)
{
	// Tell it to wrap the u coordinate
	HR(gd3dDevice->SetRenderState(D3DRS_WRAP0, D3DWRAP_U));

	BaseObject3D::Render(gd3dDevice, view, projection, lightVec, viewPos);

	// Set it back to what it was
	HR(gd3dDevice->SetRenderState(D3DRS_WRAP0, 0));
}

void Torus::Update(float dt)
{
	// We aren't changing anything, so just use the base update function
	BaseObject3D::Update(dt);
}