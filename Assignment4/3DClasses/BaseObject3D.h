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
#ifndef _BASE_OBJECT_3D_H
#define _BASE_OBJECT_3D_H
//=============================================================================
#pragma once
//=============================================================================
#include <d3dx9.h>

#include "../d3dUtil.h"
#include "../IObject.h"
#include <vector>
#include <map>
//=============================================================================
struct IDirect3DVertexBuffer9;
struct IDirect3DIndexBuffer9;
struct Vertex;
class BaseMaterial;
//=============================================================================

class BaseObject3D : public IObject
{
protected:	
    D3DXMATRIX                  m_World;
	std::map<std::string, BaseMaterial*> m_Materials; // Allows an object to have multiple materials, but only 1 active one

	std::string m_ActiveMaterial;
	std::string m_Name;

	int m_Triangles;
	int m_Vertices;
	bool m_UseMaterial;

	D3DXVECTOR3 m_CenterPos;

	LPD3DXMESH m_Mesh;
	LPD3DXBUFFER *mp_Buffer;

	virtual void generateUVs(){}; // Used by subclass (if they choose) to put their uv generation code in
	virtual void generateTBNs();

private:
	void RenderWithMaterial(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, D3DXVECTOR3& lightVec, D3DXVECTOR3& viewPos);
	void RenderWithoutMaterial(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, D3DXVECTOR3& lightVec, D3DXVECTOR3& viewPos);

public:
	BaseObject3D();
    ~BaseObject3D(void);

    // Replace or add to the following code as you progress with the material
    virtual void Create( IDirect3DDevice9* gd3dDevice ) = 0;
	virtual void Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, D3DXVECTOR3& lightVec, D3DXVECTOR3& viewPos);
	virtual void Update(float dt) {}

	virtual void dispose();

	const D3DXVECTOR3& getCenterPos() { return m_CenterPos; }
	void setCenterPos(FLOAT x, FLOAT y, FLOAT z); // Used to move a BaseObject3D to a position in World space by translating its World Matrix
	void addMaterial(std::string key, BaseMaterial* material, bool setToActive = false);
	void setActiveMaterial(std::string key) { m_ActiveMaterial = key; }
	bool getUseMaterial() { return m_UseMaterial; }
	void setUseMaterial(bool value) { m_UseMaterial = value; }
	std::string getName() { return m_Name; }
	void setName(std::string name) { m_Name = name; }
	
	BaseMaterial* getMaterial(std::string key);
	D3DXMATRIX getWorldMatrix() { return m_World;  }

	// For resetting the shaders...
	void onLostDevice();
	void onResetDevice();
};
//=============================================================================
#endif // _BASE_OBJECT_3D_H

