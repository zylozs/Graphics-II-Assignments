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
	m_TextureHandle = NULL;
	setColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_ColorHandle = NULL;
	m_UseTextureHandle = NULL;
}

TextureMaterial::TextureMaterial(std::string filename)
{
	m_TextureHandle = NULL;
	setColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_ColorHandle = NULL;
	m_UseTextureHandle = NULL;

	LoadTextureFromFile(filename);
}

TextureMaterial::~TextureMaterial()
{
	ReleaseCOM(m_Texture);
}

void TextureMaterial::ConnectToEffect(ID3DXEffect* effect)
{
	BaseMaterial::ConnectToEffect(effect);

	addTechnique("PhongTech", "Phong");
	addTechnique("GouraudTech", "Gouraud");

	m_TextureHandle = m_Effect->GetParameterByName(0, "gTexture");
	m_ColorHandle = m_Effect->GetParameterByName(0, "gColor");
	m_UseTextureHandle = m_Effect->GetParameterByName(0, "gUseTexture");
}

void TextureMaterial::PreRender(D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat, D3DXVECTOR3& lightPos, D3DXVECTOR3& viewPos)
{
	BaseMaterial::PreRender(worldMat, viewProjMat, lightPos, viewPos);

	HR(m_Effect->SetTechnique(getTechniqueHandle(m_ActiveTechnique)));
	HR(m_Effect->SetTexture(m_TextureHandle, m_Texture));
	HR(m_Effect->SetValue(m_ColorHandle, &m_Color, sizeof(D3DXCOLOR)));
	HR(m_Effect->SetBool(m_UseTextureHandle, m_UseTexture));
}

void TextureMaterial::LoadTextureFromFile(std::string filename)
{
	HR(D3DXCreateTextureFromFile(gd3dDevice, filename.c_str(), &m_Texture));
}

void TextureMaterial::setColor(FLOAT r, FLOAT g, FLOAT b, FLOAT a)
{
	m_Color = D3DXCOLOR(r, g, b, a);

	m_DiffuseColor = m_Color;
	m_SpecularColor = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	m_Shininess = 8.0f;
}

void TextureMaterial::setColor(const D3DXCOLOR& color)
{
	setColor(color.r, color.g, color.b, color.a);
}