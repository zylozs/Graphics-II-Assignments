/*
*	Vincent Loignon and Tyler Cifelli
*	Assignment 4
*	PhongTextureMaterial.h
*	I certify that this assignment is entirely our own work.
*/

#ifndef _CUBE_MAP_MATERIAL_H
#define _CUBE_MAP_MATERIAL_H

#include "BaseMaterial.h"

class CubeMapMaterial : public BaseMaterial
{
protected:
	IDirect3DCubeTexture9* m_EnvMap;
	D3DXHANDLE m_EnvMapHandle;

public:
	CubeMapMaterial();
	CubeMapMaterial(std::string envMap);
	~CubeMapMaterial();

	IDirect3DCubeTexture9* getEnvMap() { return m_EnvMap; }

	void LoadEnvMapFromFile(std::string filename, IDirect3DCubeTexture9** texture);

	virtual void ConnectToEffect(ID3DXEffect* effect);
	virtual void PreRender(D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat, D3DXVECTOR3& lightPos, D3DXVECTOR3& viewPos);
};

#endif