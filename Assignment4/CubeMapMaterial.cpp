/*
*	Vincent Loignon and Tyler Cifelli
*	Assignment 4
*	PhongTextureMaterial.cpp
*	I certify that this assignment is entirely our own work.
*/

#include "CubeMapMaterial.h"

CubeMapMaterial::CubeMapMaterial()
{
	m_EnvMap = NULL;
	m_EnvMapHandle = NULL;
}

CubeMapMaterial::CubeMapMaterial(std::string envMap)
{
	m_EnvMap = NULL;
	m_EnvMapHandle = NULL;

	LoadEnvMapFromFile(envMap, &m_EnvMap);
}

CubeMapMaterial::~CubeMapMaterial()
{
	ReleaseCOM(m_EnvMap);
}

void CubeMapMaterial::ConnectToEffect(ID3DXEffect* effect)
{
	m_Effect = effect;

	addTechnique("CubeMapTech", "CubeMap", true);

	m_EnvMapHandle = m_Effect->GetParameterByName(0, "gEnvMap");
	m_WVPMatHandle = m_Effect->GetParameterByName(0, "gWVP");
}

void CubeMapMaterial::PreRender(D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat, D3DXVECTOR3& lightPos, D3DXVECTOR3& viewPos)
{
	D3DXMATRIX wvp = worldMat * viewProjMat;

	HR(m_Effect->SetTechnique(getTechniqueHandle(m_ActiveTechnique)));
	HR(m_Effect->SetTexture(m_EnvMapHandle, m_EnvMap));
	HR(m_Effect->SetMatrix(m_WVPMatHandle, &wvp));
}

void CubeMapMaterial::LoadEnvMapFromFile(std::string filename, IDirect3DCubeTexture9** texture)
{
	HR(D3DXCreateCubeTextureFromFile(gd3dDevice, filename.c_str(), texture));
}