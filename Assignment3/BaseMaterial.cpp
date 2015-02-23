//=============================================================================
//                              BaseMaterial
//
// Writen by Adi Bar-Lev, 2013
// EGP 300-101, Graphics Programming II  - skeleton project
//
// Base class that contains the most intrinsic parameters to implement per model
// lighting / shaders effects.
//=============================================================================
#include "BaseMaterial.h"
//=============================================================================
BaseMaterial::BaseMaterial(void)
{
    m_Effect = NULL;
}

//-----------------------------------------------------------------------------
// Relase shader, blah...
BaseMaterial::~BaseMaterial(void)
{
	ReleaseCOM(m_Effect);
}

void BaseMaterial::LoadEffectFromFile(IDirect3DDevice9* gd3dDevice, std::string filename)
{
	ID3DXEffect* fx = 0;
	ID3DXBuffer* errors = 0;
	HR(D3DXCreateEffectFromFile(gd3dDevice, filename.c_str(), 0, 0, D3DXSHADER_DEBUG, 0, &fx, &errors));

	if (errors)
	{
		MessageBox(0, (char*)errors->GetBufferPointer(), 0, 0);
		return;
	}

	ConnectToEffect(fx);
}

UINT BaseMaterial::Begin()
{
	UINT passes = 0;
	HR(m_Effect->Begin(&passes, 0));

	return passes;
}

void BaseMaterial::End()
{
	HR(m_Effect->End());
}

void BaseMaterial::BeginPass(int i)
{
	HR(m_Effect->BeginPass(i));
}

void BaseMaterial::EndPass()
{
	HR(m_Effect->EndPass());
}

//=============================================================================
