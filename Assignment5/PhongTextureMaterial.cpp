/*
*	Vincent Loignon and Tyler Cifelli
*	Assignment 4
*	PhongTextureMaterial.cpp
*	I certify that this assignment is entirely our own work.
*/

#include "PhongTextureMaterial.h"

PhongTextureMaterial::PhongTextureMaterial()
{
	m_Texture = NULL;
	m_NormalMap = NULL;
	m_NormalMapHandle = NULL;
	m_TextureHandle = NULL;
}

PhongTextureMaterial::PhongTextureMaterial(std::string texture, std::string normalMap)
{
	m_Texture = NULL;
	m_NormalMap = NULL;
	m_TextureHandle = NULL;
	m_NormalMapHandle = NULL;

	LoadTextureFromFile(texture, &m_Texture);
	LoadTextureFromFile(normalMap, &m_NormalMap);
}

PhongTextureMaterial::~PhongTextureMaterial()
{
	ReleaseCOM(m_Texture);
}

void PhongTextureMaterial::ConnectToEffect(ID3DXEffect* effect)
{
	BaseMaterial::ConnectToEffect(effect);

	addTechnique("PhongTech", "Phong");

	m_TextureHandle = m_Effect->GetParameterByName(0, "gTexture");
	m_NormalMapHandle = m_Effect->GetParameterByName(0, "gNormalMap");
}

void PhongTextureMaterial::PreRender(D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat, D3DXVECTOR3& lightPos, D3DXVECTOR3& viewPos)
{
	BaseMaterial::PreRender(worldMat, viewProjMat, lightPos, viewPos);

	HR(m_Effect->SetTechnique(getTechniqueHandle(m_ActiveTechnique)));
	HR(m_Effect->SetTexture(m_TextureHandle, m_Texture));
	HR(m_Effect->SetTexture(m_NormalMapHandle, m_NormalMap));
}

void PhongTextureMaterial::LoadTextureFromFile(std::string filename, IDirect3DTexture9** texture)
{
	HR(D3DXCreateTextureFromFile(gd3dDevice, filename.c_str(), texture));
}