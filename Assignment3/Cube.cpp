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
	D3DXCreateBox(gd3dDevice, m_Width, m_Height, m_Length, &m_Mesh, mp_Buffer);

	generateUVs();
}

void Cube::generateUVs()
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

	vertices[0].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
	vertices[1].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
	vertices[2].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
	vertices[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	// Fill in the back face vertex data.
	vertices[4].TexCoord = D3DXVECTOR2(1.0f, 1.0f);
	vertices[5].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
	vertices[6].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
	vertices[7].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	// Fill in the top face vertex data.
	vertices[8].TexCoord  = D3DXVECTOR2(0.0f, 1.0f);
	vertices[9].TexCoord  = D3DXVECTOR2(0.0f, 0.0f);
	vertices[10].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
	vertices[11].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	// Fill in the bottom face vertex data.
	vertices[12].TexCoord = D3DXVECTOR2(1.0f, 1.0f);
	vertices[13].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
	vertices[14].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
	vertices[15].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

	// Fill in the left face vertex data.
	vertices[16].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
	vertices[17].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
	vertices[18].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
	vertices[19].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	// Fill in the right face vertex data.
	vertices[20].TexCoord = D3DXVECTOR2(0.0f, 1.0f);
	vertices[21].TexCoord = D3DXVECTOR2(0.0f, 0.0f);
	vertices[22].TexCoord = D3DXVECTOR2(1.0f, 0.0f);
	vertices[23].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

	// Unlock the buffer since we no longer need it
	HR(temp->UnlockVertexBuffer());

	// Clone the copy of the mesh back into the member variable with hardware friendly tags
	HR(temp->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, elements, gd3dDevice, &m_Mesh));

	// Release our local copy since we no longer need it
	ReleaseCOM(temp);
}

void Cube::Update(float dt)
{
	// We aren't changing anything, so just use the base update function
	BaseObject3D::Update(dt);
}