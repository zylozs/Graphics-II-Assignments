#ifndef _NO_LIGHT_TEXTURE_MATERIAL_H
#define _NO_LIGHT_TEXTURE_MATERIAL_H

#include "BaseMaterial.h"

class NoLightTextureMaterial : public BaseMaterial
{
protected:
	IDirect3DTexture9* m_Texture;
	D3DXHANDLE m_TextureHandle;

	IDirect3DTexture9* m_NormalMap;
	D3DXHANDLE m_NormalMapHandle;

public:
	NoLightTextureMaterial();
	NoLightTextureMaterial(std::string texture, std::string normalMap);
	~NoLightTextureMaterial();

	void LoadTextureFromFile(std::string filename, IDirect3DTexture9** texture);

	virtual void onLostDevice();
	virtual void onResetDevice();

	virtual void ConnectToEffect(ID3DXEffect* effect);
	virtual void PreRender(D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat, D3DXVECTOR3& lightPos, D3DXVECTOR3& viewPos);
};

#endif