//=============================================================================
//                              BaseObject3D
//
// Writen by Adi Bar-Lev, 2013
// EGP 300-101, Graphics Programming II  - skeleton project
//
// Base class that can handle 3D rendergin via Vertex and Index buffer
//=============================================================================
#include "BaseObject3D.h"
#include "Vertex.h"
#include "../GfxStats.h"
//=============================================================================
BaseObject3D::BaseObject3D(void)
{
    m_VertexBuffer = NULL;
    m_IndexBuffer = NULL;
	m_CenterPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    D3DXMatrixIdentity(&m_World);
}

//-----------------------------------------------------------------------------
BaseObject3D::~BaseObject3D(void)
{
    ReleaseCOM(m_VertexBuffer);
	ReleaseCOM(m_IndexBuffer);
}

//-----------------------------------------------------------------------------
void BaseObject3D::Create( IDirect3DDevice9* gd3dDevice )
{
    buildVertexBuffer( gd3dDevice );
    buildIndexBuffer( gd3dDevice );
}

//-----------------------------------------------------------------------------
void BaseObject3D::Render( IDirect3DDevice9* gd3dDevice,
    D3DXMATRIX& view, D3DXMATRIX& projection )
{
    // Update the statistics singlton class
    GfxStats::GetInstance()->addVertices(m_Vertices);
    GfxStats::GetInstance()->addTriangles(m_Triangles);

    // Set the buffers and format
    HR(gd3dDevice->SetStreamSource(0, m_VertexBuffer, 0, sizeof(VertexPos)));
	HR(gd3dDevice->SetIndices(m_IndexBuffer));
	HR(gd3dDevice->SetVertexDeclaration(VertexPos::Decl));

    // Set matrices and model relevant render date
	HR(gd3dDevice->SetTransform(D3DTS_WORLD, &m_World));
	HR(gd3dDevice->SetTransform(D3DTS_VIEW, &view));
	HR(gd3dDevice->SetTransform(D3DTS_PROJECTION, &projection));	
    
    // Send to render
    HR(gd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_Vertices, 0, m_Triangles));
}

//-----------------------------------------------------------------------------
void BaseObject3D::buildVertexBuffer( IDirect3DDevice9* gd3dDevice )
{
	std::vector<VertexPos> vertices;

	calculateVertexBuffer(vertices);

	m_Vertices = vertices.size();

	// Obtain our pointer to the Vertex Buffer
	HR(gd3dDevice->CreateVertexBuffer(m_Vertices * sizeof(VertexPos), D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &m_VertexBuffer, 0));
	VertexPos* v = 0;

	// Lock the Vertex Buffer and write to it
	HR(m_VertexBuffer->Lock(0, 0, (void**)&v, 0));

	for (UINT i = 0; i < vertices.size(); i++)
	{
		v[i] = vertices[i];
	}

	// Unlock when we are done
	HR(m_VertexBuffer->Unlock());
}

//-----------------------------------------------------------------------------
void BaseObject3D::buildIndexBuffer( IDirect3DDevice9* gd3dDevice )
{
	std::vector<WORD> indices;

	calculateIndexBuffer(indices);

	m_Triangles = indices.size() / 3;

	// Obtain our Pointer to the Index Buffer
	HR(gd3dDevice->CreateIndexBuffer(indices.size() * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_IndexBuffer, 0));
	WORD* k = 0;

	// Lock the Index Buffer and Write to it
	HR(m_IndexBuffer->Lock(0, 0, (void**)&k, 0));

	for (UINT i = 0; i < indices.size(); i++)
	{
		k[i] = indices[i];
	}

	// Unlock when we are done
	HR(m_IndexBuffer->Unlock());
}
//=============================================================================

void BaseObject3D::calculateVertexBuffer(std::vector<VertexPos>& vertices)
{
	vertices.push_back(VertexPos(-1.0f, -1.0f, -1.0f));
	vertices.push_back(VertexPos(-1.0f, 1.0f, -1.0f));
	vertices.push_back(VertexPos(1.0f, 1.0f, -1.0f));
	vertices.push_back(VertexPos(1.0f, -1.0f, -1.0f));
	vertices.push_back(VertexPos(-1.0f, -1.0f, 1.0f));
	vertices.push_back(VertexPos(-1.0f, 1.0f, 1.0f));
	vertices.push_back(VertexPos(1.0f, 1.0f, 1.0f));
	vertices.push_back(VertexPos(1.0f, -1.0f, 1.0f));
}

void BaseObject3D::calculateIndexBuffer(std::vector<WORD>& indices)
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
	indices.push_back(6); 
	indices.push_back(5);
	indices.push_back(4); 
	indices.push_back(7); 
	indices.push_back(6);

	// Left face.
	indices.push_back(4); 
	indices.push_back(5); 
	indices.push_back(1);
	indices.push_back(4); 
	indices.push_back(1); 
	indices.push_back(0);

	// Right face.
	indices.push_back(3); 
	indices.push_back(2); 
	indices.push_back(6);
	indices.push_back(3); 
	indices.push_back(6); 
	indices.push_back(7);

	// Top face.
	indices.push_back(1); 
	indices.push_back(5); 
	indices.push_back(6);
	indices.push_back(1); 
	indices.push_back(6); 
	indices.push_back(2);

	// Bottom face.
	indices.push_back(4);
	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(4);
	indices.push_back(3);
	indices.push_back(7);
}

void BaseObject3D::setCenterPos(FLOAT x, FLOAT y, FLOAT z)
{
	// Translate the object using its world matrix to the center position indicated
	D3DXMatrixTranslation(&m_World, x, y, z);

	// Set m_CenterPos to new values
	m_CenterPos.x = x;
	m_CenterPos.y = y;
	m_CenterPos.z = z;
}