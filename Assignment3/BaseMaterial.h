//=============================================================================
//                              BaseMaterial
//
// Writen by Adi Bar-Lev, 2013
// EGP 300-101, Graphics Programming II  - skeleton project
//
// Base class that contains the most intrinsic parameters to implement per model
// lighting / shaders effects.
//=============================================================================
#pragma once
#include "d3dApp.h"
#include "Trackable.h"
//=============================================================================
class BaseMaterial : public Trackable
{
protected:
    ID3DXEffect*        m_Effect;               // the shader associate effect file

    //-------- Material Parameters -------
    D3DXMATRIX          m_WorldMat;
    D3DXMATRIX          m_ViewProjectionMat;

    D3DXVECTOR3         m_DiffuseColor;
    D3DXVECTOR3         m_SpecularColor;
    float               m_Shininess;            // specular power


    //---------- Shader Handles ----------
    // Generic shader handles
    D3DXHANDLE          m_WorldMatHandle;    
    D3DXHANDLE          m_ViewProjectionMatHandle;

    D3DXHANDLE          m_LightPosWHandle;       // Position (spot/point) / Direction (directional)
    D3DXHANDLE          m_ViewerPosWHandle;

    // Material specific shader handles
    D3DXHANDLE          m_DiffuseColHandle;    
    D3DXHANDLE          m_SpecularColHandle;       
    D3DXHANDLE          m_ShininessHandle;   

	D3DXHANDLE			m_TechniqueHandle; // The technique to use for the shader

public:
    BaseMaterial(void);
    virtual ~BaseMaterial(void);

	void LoadEffectFromFile(IDirect3DDevice9* gd3dDevice, std::string filename);
	virtual void ConnectToEffect(ID3DXEffect* effect) = 0;
	virtual void PreRender(D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat) = 0;

	// For starting/ending the rendering of this material
	UINT Begin();
	void End();

	// For starting/ending the passes
	void BeginPass(int i);
	void EndPass();
};
//=============================================================================

