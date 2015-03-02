#ifndef _TEXTURE_MATERIAL_H
#define _TEXTURE_MATERIAL_H

#include "BaseMaterial.h"

class TextureMaterial : public BaseMaterial
{
protected:
	IDirect3DTexture9* m_Texture;
	D3DXHANDLE m_TextureHandle;

	BOOL m_UseTexture;
	D3DXHANDLE m_UseTextureHandle;

	D3DXCOLOR m_Color;
	D3DXHANDLE m_ColorHandle;

public:
	TextureMaterial();
	TextureMaterial(std::string filename);
	~TextureMaterial();

	void setColor(FLOAT r, FLOAT g, FLOAT b, FLOAT a);
	void setColor(const D3DXCOLOR& color);
	const D3DXCOLOR& getColor() { return m_Color; }

	void setUseTexture(BOOL value) { m_UseTexture = value; }
	BOOL getUseTexture() { return m_UseTexture; }

	void LoadTextureFromFile(std::string filename);

	virtual void ConnectToEffect(ID3DXEffect* effect);
	virtual void PreRender(D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat, D3DXVECTOR3& lightPos, D3DXVECTOR3& viewPos);
};

#endif