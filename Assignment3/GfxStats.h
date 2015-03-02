//=============================================================================
// GfxStats.h by Frank Luna (C) 2005 All Rights Reserved.
//
// Class used for keeping track of and displaying the frames rendered
// per second, milliseconds per frame, and vertex and triangle counts.

// Edited by Adi Bar-Lwv, 2013
// 
// Modified by Vincent Loignon (added new functionality)
//=============================================================================

#ifndef GFX_STATS_H
#define GFX_STATS_H

#include <d3dx9.h>
#include "d3dUtil.h"

class GfxStats
{
    DECLARE_SINGLETON(GfxStats);

public:
	~GfxStats();

	void onLostDevice();
	void onResetDevice();

	void addVertices(DWORD n);
	void subVertices(DWORD n);
	void addTriangles(DWORD n);
	void subTriangles(DWORD n);
	
	void setShader(std::string name);
	void setFillMode(DWORD mode);

	void setTriCount(DWORD n);
	void setVertexCount(DWORD n);

	void update(float dt);
	void display();

private:
	// Prevent copying
	GfxStats(const GfxStats& rhs);
	GfxStats& operator=(const GfxStats& rhs);
	
private:
	ID3DXFont* mFont;
	float mFPS;
	float mMilliSecPerFrame;
	DWORD mNumTris;
	DWORD mNumVertices;
	std::string m_ShaderName;
	std::string m_FillMode;
};
#endif // GFX_STATS_H