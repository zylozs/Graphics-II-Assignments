//=============================================================================
//                              BaseMaterial
//
// Writen by Adi Bar-Lev, 2013
// EGP 300-101, Graphics Programming II  - skeleton project
// 
// Modified by Vincent Loignon and Tyler Cifelli
//
// Base class that contains the most intrinsic parameters to implement per model
// lighting / shaders effects.
//=============================================================================
#include "BaseMaterial.h"
//=============================================================================
BaseMaterial::BaseMaterial(void)
{
    m_Effect = NULL;
	m_ActiveTechnique = "";
	m_DiffuseColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_SpecularColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_Shininess = 0.0f;
	m_UseDiffuse = true;
	m_UseSpecular = true;

	m_DiffuseColHandle = NULL;
	m_SpecularColHandle = NULL;
	m_ShininessHandle = NULL;
	m_WVPMatHandle = NULL;
	m_WorldMatHandle = NULL;
	m_WorldInvTransMatHandle = NULL;
	m_LightVecHandle = NULL;
	m_ViewerPosWHandle = NULL;
}

//-----------------------------------------------------------------------------
// Release shader, blah...
BaseMaterial::~BaseMaterial(void)
{
	ReleaseCOM(m_Effect);
}

void BaseMaterial::LoadEffectFromFile(IDirect3DDevice9* gd3dDevice, std::string filename)
{
	ID3DXEffect* fx = 0;
	ID3DXBuffer* errors = 0;
	HR(D3DXCreateEffectFromFile(gd3dDevice, filename.c_str(), 0, 0, D3DXSHADER_DEBUG, 0, &fx, &errors));

	if (errors)
	{
		MessageBox(0, (char*)errors->GetBufferPointer(), 0, 0);
		return;
	}

	ConnectToEffect(fx);
}

void BaseMaterial::ConnectToEffect(ID3DXEffect* effect)
{
	m_Effect = effect;

	m_DiffuseColHandle = m_Effect->GetParameterByName(0, "gDiffuseColor");
	m_SpecularColHandle = m_Effect->GetParameterByName(0, "gSpecularColor");
	m_ShininessHandle = m_Effect->GetParameterByName(0, "gSpecularPower");

	m_UseDiffuseHandle = m_Effect->GetParameterByName(0, "gUseDiffuse");
	m_UseSpecularHandle = m_Effect->GetParameterByName(0, "gUseSpecular");

	m_WVPMatHandle = m_Effect->GetParameterByName(0, "gWVP");
	m_WorldMatHandle = m_Effect->GetParameterByName(0, "gWorld");
	m_WorldInvTransMatHandle = m_Effect->GetParameterByName(0, "gWorldInvTrans");

	m_LightVecHandle = m_Effect->GetParameterByName(0, "gLightVec");
	m_ViewerPosWHandle = m_Effect->GetParameterByName(0, "gEyePos");
}

void BaseMaterial::PreRender(D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat, D3DXVECTOR3& lightVec, D3DXVECTOR3& viewPos)
{
	D3DXMATRIX wvp = worldMat * viewProjMat;

	D3DXMATRIX W, WIT;
	D3DXMatrixIdentity(&W);
	D3DXMatrixInverse(&WIT, 0, &W);
	D3DXMatrixTranspose(&WIT, &WIT);

	m_WorldMat = worldMat;
	m_ViewProjectionMat = viewProjMat;

	HR(m_Effect->SetBool(m_UseDiffuseHandle, m_UseDiffuse));
	HR(m_Effect->SetBool(m_UseSpecularHandle, m_UseSpecular));

	HR(m_Effect->SetValue(m_DiffuseColHandle, &m_DiffuseColor, sizeof(D3DXCOLOR)));
	HR(m_Effect->SetValue(m_SpecularColHandle, &m_SpecularColor, sizeof(D3DXCOLOR)));
	HR(m_Effect->SetFloat(m_ShininessHandle, m_Shininess));

	HR(m_Effect->SetMatrix(m_WorldMatHandle, &m_WorldMat));
	HR(m_Effect->SetMatrix(m_WorldInvTransMatHandle, &WIT));
	HR(m_Effect->SetMatrix(m_WVPMatHandle, &wvp));

	HR(m_Effect->SetValue(m_LightVecHandle, &lightVec, sizeof(D3DXVECTOR3)));
	HR(m_Effect->SetValue(m_ViewerPosWHandle, &viewPos, sizeof(D3DXVECTOR3)));
}

UINT BaseMaterial::Begin()
{
	UINT passes = 0;
	HR(m_Effect->Begin(&passes, 0));

	return passes;
}

void BaseMaterial::End()
{
	HR(m_Effect->End());
}

void BaseMaterial::BeginPass(int i)
{
	HR(m_Effect->BeginPass(i));
}

void BaseMaterial::EndPass()
{
	HR(m_Effect->EndPass());
}

D3DXHANDLE BaseMaterial::getTechniqueHandle(std::string key)
{
	std::map<std::string, D3DXHANDLE>::iterator it = m_TechniqueHandles.find(key);

	if (it == m_TechniqueHandles.end())
		return NULL;
	else
		return it->second;
}

void BaseMaterial::addTechnique(std::string name, std::string key /* = "" */, bool setToActive /* = false */)
{
	D3DXHANDLE technique = m_Effect->GetTechniqueByName(name.c_str());

	std::string newKey = key;

	if (key == "")
		newKey = name;

	m_TechniqueHandles[newKey] = technique;

	if (setToActive)
		setActiveTechnique(newKey);
}

void BaseMaterial::onLostDevice()
{
	HR(m_Effect->OnLostDevice());
}

void BaseMaterial::onResetDevice()
{
	HR(m_Effect->OnResetDevice());
}

//=============================================================================