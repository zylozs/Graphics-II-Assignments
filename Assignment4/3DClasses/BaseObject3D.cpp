//=============================================================================
//                              BaseObject3D
//
// Writen by Adi Bar-Lev, 2013
// EGP 300-101, Graphics Programming II  - skeleton project
// 
// Modified by Vincent Loignon
// Added more modularity to it
//
// Base class that can handle 3D rendering via D3DXMESHs
//=============================================================================
#include "BaseObject3D.h"
#include "Vertex.h"
#include "../BaseMaterial.h"
#include "../GfxStats.h"
//=============================================================================
BaseObject3D::BaseObject3D()
{
	m_CenterPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    D3DXMatrixIdentity(&m_World);
	mp_Buffer = NULL;
	m_ActiveMaterial = "";
	m_UseMaterial = true;
	m_Name = "BaseObject3D";
}

//-----------------------------------------------------------------------------
BaseObject3D::~BaseObject3D(void)
{
	
}

void BaseObject3D::dispose()
{
	ReleaseCOM(m_Mesh);

	std::map<std::string, BaseMaterial*>::iterator it = m_Materials.begin();

	for (it; it != m_Materials.end(); it++)
	{
		delete it->second;
		it->second = NULL;
	}

	m_Materials.clear();
}

//-----------------------------------------------------------------------------
void BaseObject3D::Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, D3DXVECTOR3& lightVec, D3DXVECTOR3& viewPos)
{
    // Update the statistics singleton class
    GfxStats::GetInstance()->addVertices(m_Mesh->GetNumVertices());
	//this might not work
    GfxStats::GetInstance()->addTriangles(m_Mesh->GetNumFaces());

    // Set matrices and model relevant render date
	HR(gd3dDevice->SetTransform(D3DTS_WORLD, &m_World));
	HR(gd3dDevice->SetTransform(D3DTS_VIEW, &view));
	HR(gd3dDevice->SetTransform(D3DTS_PROJECTION, &projection));	

	if (m_UseMaterial && m_Materials.size() > 0)
		RenderWithMaterial(gd3dDevice, view, projection, lightVec, viewPos);
	else
		RenderWithoutMaterial(gd3dDevice, view, projection, lightVec, viewPos);
}

void BaseObject3D::RenderWithMaterial(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, D3DXVECTOR3& lightVec, D3DXVECTOR3& viewPos)
{
	BaseMaterial* material = getMaterial(m_ActiveMaterial);

	if (material == NULL)
	{
		std::cerr << "Warning: You have no Active Material but are trying to one. Assign a material or disable their use.\n";
		RenderWithoutMaterial(gd3dDevice, view, projection, lightVec, viewPos);

		return;
	}

	material->PreRender(m_World, view * projection, lightVec, viewPos);

	UINT passes = material->Begin();

	for (int i = 0; i < (int)passes; i++)
	{
		material->BeginPass(i);

		m_Mesh->DrawSubset(0);

		material->EndPass();
	}

	material->End();
}

void BaseObject3D::RenderWithoutMaterial(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, D3DXVECTOR3& lightVec, D3DXVECTOR3& viewPos)
{
	// Send to render
	m_Mesh->DrawSubset(0);
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

void BaseObject3D::addMaterial(std::string key, BaseMaterial* material, bool setToActive /* = false*/)
{
	std::map<std::string, BaseMaterial*>::iterator it = m_Materials.find(key);

	if (it != m_Materials.end())
		delete m_Materials[key];

	m_Materials[key] = material;

	if (setToActive)
		m_ActiveMaterial = key;
}

BaseMaterial* BaseObject3D::getMaterial(std::string key)
{
	std::map<std::string, BaseMaterial*>::iterator it = m_Materials.find(key);

	if (it == m_Materials.end())
		return NULL;
	else
		return it->second;
}

void BaseObject3D::onLostDevice()
{
	std::map<std::string, BaseMaterial*>::iterator it = m_Materials.begin();

	for (it; it != m_Materials.end(); it++)
	{
		it->second->onLostDevice();
	}
}

void BaseObject3D::onResetDevice()
{
	std::map<std::string, BaseMaterial*>::iterator it = m_Materials.begin();

	for (it; it != m_Materials.end(); it++)
	{
		it->second->onResetDevice();
	}
}

void BaseObject3D::generateTBNs()
{
	// Grab our vertex description
	D3DVERTEXELEMENT9 elements[64];
	UINT numElements = 0;
	Vertex::Decl->GetDeclaration(elements, &numElements);

	// Create a copy of our sphere mesh using our vertex description instead of the old one
	ID3DXMesh* temp = 0;
	HR(m_Mesh->CloneMesh(D3DXMESH_MANAGED, elements, gd3dDevice, &temp));

	// Release our old mesh since we have a copy we will be modifying in system memory
	ReleaseCOM(m_Mesh);

	HR(D3DXComputeTangentFrameEx(
		temp, // Input mesh
		D3DDECLUSAGE_TEXCOORD, 0, // Vertex element of input tex-coords.  
		D3DDECLUSAGE_BINORMAL, 0, // Vertex element to output binormal.
		D3DDECLUSAGE_TANGENT, 0,  // Vertex element to output tangent.
		D3DDECLUSAGE_NORMAL, 0,   // Vertex element to output normal.
		0, // Options
		0, // Adjacency
		0.01f, 0.25f, 0.01f, // Thresholds for handling errors
		&m_Mesh, // Output mesh
		0));         // Vertex Remapping

	// Clone the copy of the mesh back into the member variable with hardware friendly tags
	//HR(temp->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, elements, gd3dDevice, &m_Mesh));

	// Release our local copy since we no longer need it
	ReleaseCOM(temp);
}