#include "ColorMaterial.h"

ColorMaterial::ColorMaterial(FLOAT r, FLOAT g, FLOAT b, FLOAT a)
{
	setColor(r, g, b, a);
	m_ColorHandle = NULL;
}

ColorMaterial::ColorMaterial(const D3DXCOLOR& color)
{
	setColor(color);
	m_ColorHandle = NULL;
}

ColorMaterial::~ColorMaterial()
{

}

void ColorMaterial::ConnectToEffect(ID3DXEffect* effect)
{
	BaseMaterial::ConnectToEffect(effect);

	addTechnique("ColorTech", "Color", true);
	addTechnique("ColorPhongTech", "Phong");
	addTechnique("ColorGouraudTech", "Gouraud");

	m_ColorHandle = m_Effect->GetParameterByName(0, "gColor");
}

void ColorMaterial::PreRender(D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat, D3DXVECTOR3& lightPos, D3DXVECTOR3& viewPos)
{
	BaseMaterial::PreRender(worldMat, viewProjMat, lightPos, viewPos);

	HR(m_Effect->SetTechnique(getTechniqueHandle(m_ActiveTechnique)));
	HR(m_Effect->SetValue(m_ColorHandle, &m_Color, sizeof(D3DXCOLOR)));
}

void ColorMaterial::setColor(FLOAT r, FLOAT g, FLOAT b, FLOAT a)
{
	m_Color = D3DXCOLOR(r, g, b, a);

	m_DiffuseColor = m_Color;
	m_SpecularColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_Shininess = 64.0f;
}

void ColorMaterial::setColor(const D3DXCOLOR& color)
{
	setColor(color.r, color.g, color.b, color.a);
}