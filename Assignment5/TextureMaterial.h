/*
*	Vincent Loignon and Tyler Cifelli
*	Assignment 4
*	TextureMaterial.h
*	I certify that this assignment is entirely our own work.
*/

#ifndef _TEXTURE_MATERIAL_H
#define _TEXTURE_MATERIAL_H

#include "BaseMaterial.h"

class TextureMaterial : public BaseMaterial
{
protected:
	IDirect3DTexture9* m_Texture;
	D3DXHANDLE m_TextureHandle;

	IDirect3DTexture9* m_NormalMap;
	D3DXHANDLE m_NormalMapHandle;

	IDirect3DCubeTexture9* m_EnvMap;
	D3DXHANDLE m_EnvMapHandle;

	BOOL m_UseTexture;
	D3DXHANDLE m_UseTextureHandle;

	BOOL m_UseEnvMap;
	D3DXHANDLE m_UseEnvMapHandle;

	FLOAT m_EnvMapStrength;
	D3DXHANDLE m_EnvMapStrHandle;

	BOOL m_UseNormalMap;
	D3DXHANDLE m_UseNormalMapHandle;

	FLOAT m_NormalMapStr;
	D3DXHANDLE m_NormalMapStrHandle;

public:
	TextureMaterial();
	TextureMaterial(std::string texture, std::string normalMap, std::string envMap);
	~TextureMaterial();

	void setUseTexture(BOOL value) { m_UseTexture = value; }
	BOOL getUseTexture() { return m_UseTexture; }

	void setUseEnvMap(BOOL value) { m_UseEnvMap = value; }
	BOOL getUseEnvMap() { return m_UseEnvMap; }

	void setEnvMapStr(FLOAT value) { m_EnvMapStrength = value; }
	FLOAT getEnvMapStr() { return m_EnvMapStrength; }

	void setUseNormalMap(BOOL value) { m_UseNormalMap = value; }
	BOOL getUseNormalMap() { return m_UseNormalMap; }

	void setNormalMapStr(FLOAT value) { m_NormalMapStr = value; }
	FLOAT getNormalMapStr() { return m_NormalMapStr; }

	void LoadTextureFromFile(std::string filename, IDirect3DTexture9** texture);
	void LoadEnvMapFromFile(std::string envMap);

	virtual void ConnectToEffect(ID3DXEffect* effect);
	virtual void PreRender(D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat, D3DXVECTOR3& lightPos, D3DXVECTOR3& viewPos);
};

#endif