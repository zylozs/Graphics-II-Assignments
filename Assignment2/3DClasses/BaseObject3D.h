//=============================================================================
//                              BaseObject3D
//
// Writen by Adi Bar-Lev, 2013
// EGP 300-101, Graphics Programming II  - skeleton project
//
// Base class that can handle 3D rendergin via Vertex and Index buffer
//=============================================================================
#ifndef _BASE_OBJECT_3D_H
#define _BASE_OBJECT_3D_H
//=============================================================================
#pragma once
//=============================================================================
#include <d3dx9.h>

#include "../d3dUtil.h"
#include <vector>
//=============================================================================
struct IDirect3DVertexBuffer9;
struct IDirect3DIndexBuffer9;
struct VertexPos;
//=============================================================================

#define PI 3.14159265358979323846

class BaseObject3D
{
protected:	
    D3DXMATRIX                  m_World;

	IDirect3DVertexBuffer9*     m_VertexBuffer;
	IDirect3DIndexBuffer9*      m_IndexBuffer;

	int m_Triangles;
	int m_Vertices;

	D3DXVECTOR3 m_CenterPos;


protected:
    // Replace the code in the following methods
    virtual void buildVertexBuffer( IDirect3DDevice9* gd3dDevice );
    virtual void buildIndexBuffer( IDirect3DDevice9* gd3dDevice );

	virtual void calculateVertexBuffer(std::vector<VertexPos>& vertices);
	virtual void calculateIndexBuffer(std::vector<WORD>& indices);

public:
    BaseObject3D(void);
    ~BaseObject3D(void);

    // Replace or add to the following code as you progress with the material
    virtual void Create( IDirect3DDevice9* gd3dDevice );
    virtual void Render( IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection );
	virtual void Update(float dt) {}

	const D3DXVECTOR3& getCenterPos() { return m_CenterPos; }
	void setCenterPos(FLOAT x, FLOAT y, FLOAT z);
};
//=============================================================================
#endif // _BASE_OBJECT_3D_H

