//=============================================================================
//                              BaseMaterial
//
// Writen by Adi Bar-Lev, 2013
// EGP 300-101, Graphics Programming II  - skeleton project
// 
// Modified by Vincent Loignon and Tyler Cifelli
//
// Base class that contains the most intrinsic parameters to implement per model
// lighting / shaders effects.
//=============================================================================
#pragma once
#include "d3dApp.h"
#include "Trackable.h"
#include <map>
#include "Common.h"

//=============================================================================
class BaseMaterial : public Trackable
{
protected:
	ID3DXEffect*        m_Effect;               // the shader associate effect file

	//-------- Material Parameters -------
	D3DXMATRIX          m_WorldMat;
	D3DXMATRIX          m_ViewProjectionMat;

	MaterialColor		m_Colors;

	//---------- Shader Handles ----------
	// Generic shader handles
	D3DXHANDLE          m_WorldMatHandle;
	D3DXHANDLE          m_WVPMatHandle;
	D3DXHANDLE			m_WorldInvTransMatHandle;

	D3DXHANDLE          m_LightVecHandle;       // Position (spot/point) / Direction (directional)
	D3DXHANDLE          m_ViewerPosWHandle;

	D3DXHANDLE			m_AmbientHandle;
	D3DXHANDLE			m_DiffuseHandle;
	D3DXHANDLE			m_SpecularHandle;
	D3DXHANDLE			m_ShininessHandle;

	std::map<std::string, D3DXHANDLE> m_TechniqueHandles; // All the techniques we should know about for this shader
	std::string m_ActiveTechnique;

public:
	BaseMaterial(void);
	virtual ~BaseMaterial(void);

	void LoadEffectFromFile(IDirect3DDevice9* gd3dDevice, std::string filename);
	virtual void ConnectToEffect(ID3DXEffect* effect);
	virtual void PreRender(D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat, D3DXVECTOR3& lightVec, D3DXVECTOR3& viewPos);

	std::string getActiveTechnique() { return m_ActiveTechnique; }
	void setActiveTechnique(std::string key) { m_ActiveTechnique = key; }

	D3DXHANDLE getTechniqueHandle(std::string key);
	void addTechnique(std::string name, std::string key = "", bool setToActive = false);

	D3DXCOLOR getDiffuse() { return m_Colors.diffuse; }
	void setDiffuse(D3DXCOLOR value) { m_Colors.diffuse = value; }

	D3DXCOLOR getAmbient() { return m_Colors.ambient; }
	void setAmbient(D3DXCOLOR value) { m_Colors.ambient = value; }

	D3DXCOLOR getSpecular() { return m_Colors.specular; }
	void setSpecular(D3DXCOLOR value) { m_Colors.specular = value; }

	FLOAT getShinines() { return m_Colors.shininess; }
	void setShininess(FLOAT value) { m_Colors.shininess = value; }

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

