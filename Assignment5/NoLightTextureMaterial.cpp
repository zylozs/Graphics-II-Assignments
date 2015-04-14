/*
*	Vincent Loignon and Tyler Cifelli
*	Assignment 4
*	PhongTextureMaterial.cpp
*	I certify that this assignment is entirely our own work.
*/

#include "NoLightTextureMaterial.h"

NoLightTextureMaterial::NoLightTextureMaterial()
{
	m_Texture = NULL;
	m_NormalMap = NULL;
	m_NormalMapHandle = NULL;
	m_TextureHandle = NULL;
}

NoLightTextureMaterial::NoLightTextureMaterial(std::string texture, std::string normalMap)
{
	m_Texture = NULL;
	m_NormalMap = NULL;
	m_TextureHandle = NULL;
	m_NormalMapHandle = NULL;

	LoadTextureFromFile(texture, &m_Texture);
	LoadTextureFromFile(normalMap, &m_NormalMap);
}

NoLightTextureMaterial::~NoLightTextureMaterial()
{
	ReleaseCOM(m_Texture);
}

void NoLightTextureMaterial::onLostDevice()
{
	HR(m_Effect->OnLostDevice());
}

void NoLightTextureMaterial::onResetDevice()
{
	HR(m_Effect->OnResetDevice());
}

void NoLightTextureMaterial::ConnectToEffect(ID3DXEffect* effect)
{
	BaseMaterial::ConnectToEffect(effect);

	addTechnique("NoLightTextureTech", "NoLightTexture");

	m_TextureHandle = m_Effect->GetParameterByName(0, "gTexture");
	m_NormalMapHandle = m_Effect->GetParameterByName(0, "gNormalMap");
}

void NoLightTextureMaterial::PreRender(D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat, D3DXVECTOR3& lightPos, D3DXVECTOR3& viewPos)
{
	BaseMaterial::PreRender(worldMat, viewProjMat, lightPos, viewPos);

	HR(m_Effect->SetTechnique(getTechniqueHandle(m_ActiveTechnique)));
	HR(m_Effect->SetTexture(m_TextureHandle, m_Texture));
	HR(m_Effect->SetTexture(m_NormalMapHandle, m_NormalMap));
}

void NoLightTextureMaterial::LoadTextureFromFile(std::string filename, IDirect3DTexture9** texture)
{
	HR(D3DXCreateTextureFromFile(gd3dDevice, filename.c_str(), texture));
}