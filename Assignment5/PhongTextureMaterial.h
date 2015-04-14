/*
*	Vincent Loignon and Tyler Cifelli
*	Assignment 4
*	PhongTextureMaterial.h
*	I certify that this assignment is entirely our own work.
*/

#ifndef _PHONG_TEXTURE_MATERIAL_H
#define _PHONG_TEXTURE_MATERIAL_H

#include "BaseMaterial.h"

class PointLight;

class PhongTextureMaterial : public BaseMaterial
{
protected:
	IDirect3DTexture9* m_Texture;
	D3DXHANDLE m_TextureHandle;

	IDirect3DTexture9* m_NormalMap;
	D3DXHANDLE m_NormalMapHandle;

	D3DXHANDLE m_AttenuationHandle;

	BOOL m_UsePointLight;

	PointLight* m_Light;

public:
	PhongTextureMaterial();
	PhongTextureMaterial(std::string texture, std::string normalMap, PointLight* pLight = NULL);
	~PhongTextureMaterial();

	BOOL getUsePointLight() { return m_UsePointLight; }
	void setUsePointLight(BOOL value);

	void setPointLight(PointLight* pLight);
	PointLight* getPointLight() { return m_Light; }

	void LoadTextureFromFile(std::string filename, IDirect3DTexture9** texture);

	virtual void onLostDevice();
	virtual void onResetDevice();

	virtual void ConnectToEffect(ID3DXEffect* effect);
	virtual void PreRender(D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat, D3DXVECTOR3& lightPos, D3DXVECTOR3& viewPos);
};

#endif