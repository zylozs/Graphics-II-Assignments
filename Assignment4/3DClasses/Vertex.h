//=============================================================================
//              Adopted from Franc Luna's CubeDemo sample project.
//
// Please use this file to add classes / inherit and create various vertex classes
//=============================================================================
#ifndef VERTEX_H
#define VERTEX_H
//=============================================================================
#include <d3dx9.h>
//=============================================================================
// Call in constructor and destructor, respectively, of derived application class.
void InitAllVertexDeclarations();
void DestroyAllVertexDeclarations();
//=============================================================================

struct Vertex
{
	Vertex(){}
	Vertex(const D3DXVECTOR3& p, const D3DXVECTOR3& n, const D3DXVECTOR3& uv)
		   : Position(p), Normal(n), TexCoord(uv){}
	Vertex(float px, float py, float pz,
		   float nx, float ny, float nz,
		   float u, float v, 
		   float tx = 0.0f, float ty = 0.0f, float tz = 0.0f)
		   : Position(px, py, pz), Normal(nx, ny, nz), TexCoord(u, v), Tangent(tx, ty, tz){}

	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
	D3DXVECTOR3 Tangent;
	D3DXVECTOR2 TexCoord;

	static IDirect3DVertexDeclaration9* Decl;
};
//=============================================================================
#endif // VERTEX_H