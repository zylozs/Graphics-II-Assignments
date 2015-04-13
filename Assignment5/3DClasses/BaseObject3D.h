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
    D3DXMATRIX m_World;
	std::map<std::string, BaseMaterial*> m_Materials; // Allows an object to have multiple materials, but only 1 active one

	std::string m_ActiveMaterial;
	std::string m_Name;

	int m_Triangles;
	int m_Vertices;
	bool m_UseMaterial;

	//////////////////////////////////////////////////////////////////////////
	// Tree stuff

	BaseObject3D* m_Parent;
	std::vector<BaseObject3D*> m_Children;

	bool childrenContains(BaseObject3D* child);
	void removeFromChildList(BaseObject3D* child);

	//////////////////////////////////////////////////////////////////////////

	D3DXVECTOR3 m_Position;
	D3DXVECTOR3 m_Rotation; // Euler Rotation value (to use for parenting)

	LPD3DXMESH m_Mesh;
	LPD3DXBUFFER *mp_Buffer;

	virtual void generateUVs(){}; // Used by subclass (if they choose) to put their uv generation code in
	virtual void generateTBNs();
	virtual void updateChildren(float dt);
	virtual void calculateWorldMatrix();

private:
	void RenderWithMaterial(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, D3DXVECTOR3& lightVec, D3DXVECTOR3& viewPos);
	void RenderWithoutMaterial(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, D3DXVECTOR3& lightVec, D3DXVECTOR3& viewPos);

public:
	BaseObject3D();
    ~BaseObject3D(void);

    // Replace or add to the following code as you progress with the material
    virtual void Create( IDirect3DDevice9* gd3dDevice ) = 0;
	virtual void Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, D3DXVECTOR3& lightVec, D3DXVECTOR3& viewPos);
	virtual void Update(float dt);
	virtual void dispose();

	void setPosition(FLOAT x, FLOAT y, FLOAT z); // Used to move a BaseObject3D to a position in World space by translating its World Matrix
	void addToPosition(FLOAT x, FLOAT y, FLOAT z);
	void setRotation(FLOAT x, FLOAT y, FLOAT z);
	void addToRotation(FLOAT x, FLOAT y, FLOAT z);
	virtual void addMaterial(std::string key, BaseMaterial* material, bool setToActive = false);
	virtual void setActiveMaterial(std::string key) { m_ActiveMaterial = key; }
	virtual void setUseMaterial(bool value) { m_UseMaterial = value; }
	void setName(std::string name) { m_Name = name; }
	
	virtual BaseMaterial* getMaterial(std::string key);
	D3DXMATRIX getWorldMatrix() { return m_World; }
	virtual bool getUseMaterial() { return m_UseMaterial; }
	std::string getName() { return m_Name; }
	const D3DXVECTOR3& getPosition() { return m_Position; }
	const D3DXVECTOR3& getRotation() { return m_Rotation; }

	//////////////////////////////////////////////////////////////////////////
	// Tree stuff
	bool isRoot() { return (m_Parent == NULL); }
	bool isAttached() { return !isRoot(); }

	void addChild(BaseObject3D* child);
	void addChildren(std::vector<BaseObject3D*> children);
	void removeChild(BaseObject3D* child);

	// Getters
	BaseObject3D* getParent() { return m_Parent; }
	std::vector<BaseObject3D*> getChildren() { return m_Children; }

	// Setters
	void setParent(BaseObject3D* parent) { m_Parent = parent; }
	void setChildren(std::vector<BaseObject3D*> children) { m_Children = children; }
	//////////////////////////////////////////////////////////////////////////

	// For resetting the shaders...
	void onLostDevice();
	void onResetDevice();
};
//=============================================================================
#endif // _BASE_OBJECT_3D_H

