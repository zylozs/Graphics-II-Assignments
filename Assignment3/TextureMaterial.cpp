#include "TextureMaterial.h"

TextureMaterial::TextureMaterial()
{
	m_Texture = NULL;
	m_TextureHandle = NULL;
}

TextureMaterial::TextureMaterial(std::string filename)
{
	m_TextureHandle = NULL;

	LoadTextureFromFile(filename);
}

TextureMaterial::~TextureMaterial()
{
	ReleaseCOM(m_Texture);
}

void TextureMaterial::ConnectToEffect(ID3DXEffect* effect)
{
	BaseMaterial::ConnectToEffect(effect);

	addTechnique("TextureMapTech", "TextureMap", true);

	m_TextureHandle = m_Effect->GetParameterByName(0, "gTexture");
}

void TextureMaterial::PreRender(D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat, D3DXVECTOR3& lightPos, D3DXVECTOR3& viewPos)
{
	BaseMaterial::PreRender(worldMat, viewProjMat, lightPos, viewPos);

	HR(m_Effect->SetTechnique(getTechniqueHandle(m_ActiveTechnique)));
	HR(m_Effect->SetTexture(m_TextureHandle, m_Texture));
}

void TextureMaterial::LoadTextureFromFile(std::string filename)
{
	HR(D3DXCreateTextureFromFile(gd3dDevice, filename.c_str(), &m_Texture));
}