#ifndef _TEXTURE_MATERIAL_H
#define _TEXTURE_MATERIAL_H

#include "BaseMaterial.h"

class TextureMaterial : public BaseMaterial
{
protected:
	IDirect3DTexture9* m_Texture;
	D3DXHANDLE m_TextureHandle;

public:
	TextureMaterial();
	TextureMaterial(std::string filename);
	~TextureMaterial();

	void LoadTextureFromFile(std::string filename);

	virtual void ConnectToEffect(ID3DXEffect* effect);
	virtual void PreRender(D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat, D3DXVECTOR3& lightPos, D3DXVECTOR3& viewPos);
};

#endif