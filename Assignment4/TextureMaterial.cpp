/*
*	Vincent Loignon and Tyler Cifelli
*	Assignment 3
*	TextureMaterial.cpp
*	I certify that this assignment is entirely our own work.
*/

#include "TextureMaterial.h"

TextureMaterial::TextureMaterial()
{
	m_Texture = NULL;
	m_NormalMap = NULL;
	m_EnvMap = NULL;
	m_NormalMapHandle = NULL;
	m_TextureHandle = NULL;
	m_EnvMapHandle = NULL;
	m_UseTextureHandle = NULL;
	m_UseEnvMapHandle = NULL;
	m_EnvMapStrHandle = NULL;
	m_UseNormalMapHandle = NULL;
	m_NormalMapStrHandle = NULL;
}

TextureMaterial::TextureMaterial(std::string texture, std::string normalMap, std::string envMap)
{
	m_Texture = NULL;
	m_NormalMap = NULL;
	m_EnvMap = NULL;
	m_TextureHandle = NULL;
	m_NormalMapHandle = NULL;
	m_EnvMapHandle = NULL;
	m_UseTextureHandle = NULL;
	m_UseEnvMapHandle = NULL;
	m_EnvMapStrHandle = NULL;
	m_UseNormalMapHandle = NULL;
	m_NormalMapStrHandle = NULL;

	LoadTextureFromFile(texture, &m_Texture);
	LoadTextureFromFile(normalMap, &m_NormalMap);
	LoadEnvMapFromFile(envMap);
}

TextureMaterial::~TextureMaterial()
{
	ReleaseCOM(m_Texture);
}

void TextureMaterial::ConnectToEffect(ID3DXEffect* effect)
{
	BaseMaterial::ConnectToEffect(effect);

	addTechnique("PhongTech", "Phong");

	m_TextureHandle = m_Effect->GetParameterByName(0, "gTexture");
	m_EnvMapHandle = m_Effect->GetParameterByName(0, "gEnvMap");
	m_NormalMapHandle = m_Effect->GetParameterByName(0, "gNormalMap");
	m_UseTextureHandle = m_Effect->GetParameterByName(0, "gUseTexture");
	m_UseEnvMapHandle = m_Effect->GetParameterByName(0, "gUseEnvMap");
	m_EnvMapStrHandle = m_Effect->GetParameterByName(0, "gEnvMapStr");
	m_UseNormalMapHandle = m_Effect->GetParameterByName(0, "gUseNormalMap");
	m_NormalMapStrHandle = m_Effect->GetParameterByName(0, "gNormalMapStr");
}

void TextureMaterial::PreRender(D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat, D3DXVECTOR3& lightPos, D3DXVECTOR3& viewPos)
{
	BaseMaterial::PreRender(worldMat, viewProjMat, lightPos, viewPos);

	HR(m_Effect->SetTechnique(getTechniqueHandle(m_ActiveTechnique)));
	HR(m_Effect->SetTexture(m_TextureHandle, m_Texture));
	HR(m_Effect->SetTexture(m_EnvMapHandle, m_EnvMap));
	HR(m_Effect->SetTexture(m_NormalMapHandle, m_NormalMap));
	HR(m_Effect->SetBool(m_UseTextureHandle, m_UseTexture));
	HR(m_Effect->SetBool(m_UseEnvMapHandle, m_UseEnvMap));
	HR(m_Effect->SetBool(m_UseNormalMapHandle, m_UseNormalMap));
	HR(m_Effect->SetFloat(m_EnvMapStrHandle, m_EnvMapStrength));
	HR(m_Effect->SetFloat(m_NormalMapStrHandle, m_NormalMapStr));
}

void TextureMaterial::LoadTextureFromFile(std::string filename, IDirect3DTexture9** texture)
{
	HR(D3DXCreateTextureFromFile(gd3dDevice, filename.c_str(), texture));
}

void TextureMaterial::LoadEnvMapFromFile(std::string envMap)
{
	HR(D3DXCreateCubeTextureFromFile(gd3dDevice, envMap.c_str(), &m_EnvMap));
}