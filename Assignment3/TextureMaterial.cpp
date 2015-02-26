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
	m_Effect = effect;

	m_TextureHandle = m_Effect->GetParameterByName(0, "gTexture");
	m_WorldMatHandle = m_Effect->GetParameterByName(0, "gWorldViewProjectMatrix");
	m_TechniqueHandle = m_Effect->GetTechniqueByName("TextureMapTech");
}

void TextureMaterial::PreRender(D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat)
{
	m_WorldMat = worldMat * viewProjMat;

	HR(m_Effect->SetTechnique(m_TechniqueHandle));
	HR(m_Effect->SetMatrix(m_WorldMatHandle, &m_WorldMat));
	HR(m_Effect->SetTexture(m_TextureHandle, m_Texture));
}

void TextureMaterial::LoadTextureFromFile(std::string filename)
{
	HR(D3DXCreateTextureFromFile(gd3dDevice, filename.c_str(), &m_Texture));
}