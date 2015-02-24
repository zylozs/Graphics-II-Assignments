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
		   : Position(p), Normal(n), TexC(uv){}
	Vertex(float px, float py, float pz,
		   float nx, float ny, float nz,
		   float u, float v)
		   : Position(px, py, pz), Normal(nx, ny, nz), TexC(u, v){}

	D3DXVECTOR3 Position;
	D3DXVECTOR3 Normal;
	D3DXVECTOR2 TexC;

	static IDirect3DVertexDeclaration9* Decl;
};
//=============================================================================
#endif // VERTEX_H