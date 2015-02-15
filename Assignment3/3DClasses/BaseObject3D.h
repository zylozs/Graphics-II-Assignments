//=============================================================================
//                              BaseObject3D
//
// Writen by Adi Bar-Lev, 2013
// EGP 300-101, Graphics Programming II  - skeleton project
// 
// Modified by Vincent Loignon, 2015
// Added more modularity to it
//
// Base class that can handle 3D rendering via Vertex and Index buffer
//=============================================================================
#ifndef _BASE_OBJECT_3D_H
#define _BASE_OBJECT_3D_H
//=============================================================================
#pragma once
//=============================================================================
#include <d3dx9.h>

#include "../d3dUtil.h"
#include "../Trackable.h"
#include <vector>
//=============================================================================
struct IDirect3DVertexBuffer9;
struct IDirect3DIndexBuffer9;
struct Vertex;
//=============================================================================

class BaseObject3D : public Trackable
{
protected:	
    D3DXMATRIX                  m_World;

	IDirect3DVertexBuffer9*     m_VertexBuffer;
	IDirect3DIndexBuffer9*      m_IndexBuffer;

	int m_Triangles;
	int m_Vertices;

	D3DXVECTOR3 m_CenterPos;


protected:
    // Can be over-written, but isn't necessary for sub classes
    virtual void buildVertexBuffer( IDirect3DDevice9* gd3dDevice );
    virtual void buildIndexBuffer( IDirect3DDevice9* gd3dDevice );

	// You should override these functions for each sub class!
	// Add your vertices/indices to the vector passed to the function and it will draw the object for you
	virtual void calculateVertexBuffer(std::vector<Vertex>& vertices) = 0;
	virtual void calculateIndexBuffer(std::vector<WORD>& indices) = 0;

public:
    BaseObject3D(void);
    ~BaseObject3D(void);

    // Replace or add to the following code as you progress with the material
    virtual void Create( IDirect3DDevice9* gd3dDevice );
    virtual void Render( IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection );
	virtual void Update(float dt) {}

	const D3DXVECTOR3& getCenterPos() { return m_CenterPos; }
	void setCenterPos(FLOAT x, FLOAT y, FLOAT z); // Used to move a BaseObject3D to a position in World space by translating its World Matrix
};
//=============================================================================
#endif // _BASE_OBJECT_3D_H

