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
#include <map>

//=============================================================================
class BaseMaterial : public Trackable
{
protected:
	ID3DXEffect*        m_Effect;               // the shader associate effect file

	//-------- Material Parameters -------
	D3DXMATRIX          m_WorldMat;
	D3DXMATRIX          m_ViewProjectionMat;

	D3DXCOLOR			m_DiffuseColor;
	D3DXCOLOR			m_SpecularColor;
	FLOAT               m_Shininess;            // specular power


	//---------- Shader Handles ----------
	// Generic shader handles
	D3DXHANDLE          m_WorldMatHandle;
	D3DXHANDLE          m_WVPMatHandle;
	D3DXHANDLE			m_WorldInvTransMatHandle;

	D3DXHANDLE          m_LightPosWHandle;       // Position (spot/point) / Direction (directional)
	D3DXHANDLE          m_ViewerPosWHandle;

	// Material specific shader handles
	D3DXHANDLE          m_DiffuseColHandle;
	D3DXHANDLE          m_SpecularColHandle;
	D3DXHANDLE          m_ShininessHandle;

	D3DXHANDLE			m_UseDiffuseHandle;
	D3DXHANDLE			m_UseSpecularHandle;

	std::map<std::string, D3DXHANDLE> m_TechniqueHandles; // All the techniques we should know about for this shader
	std::string m_ActiveTechnique;

	BOOL m_UseDiffuse;
	BOOL m_UseSpecular;

public:
	BaseMaterial(void);
	virtual ~BaseMaterial(void);

	void LoadEffectFromFile(IDirect3DDevice9* gd3dDevice, std::string filename);
	virtual void ConnectToEffect(ID3DXEffect* effect);
	virtual void PreRender(D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat, D3DXVECTOR3& lightPos, D3DXVECTOR3& viewPos);

	std::string getActiveTechnique() { return m_ActiveTechnique; }
	void setActiveTechnique(std::string key) { m_ActiveTechnique = key; }

	D3DXHANDLE getTechniqueHandle(std::string key);
	void addTechnique(std::string name, std::string key = "", bool setToActive = false);

	bool getUseDiffuse() { return m_UseDiffuse; }
	bool getUseSpecular() { return m_UseSpecular; }

	void setUseDiffuse(BOOL value) { m_UseDiffuse = value; }
	void setUseSpecular(BOOL value) { m_UseSpecular = value; }

	void onLostDevice();
	void onResetDevice();

	// For starting/ending the rendering of this material
	UINT Begin();
	void End();

	// For starting/ending the passes
	void BeginPass(int i);
	void EndPass();
};
//=============================================================================

