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
	//m_Effect = effect;
	BaseMaterial::ConnectToEffect(effect);

	m_ColorHandle = m_Effect->GetParameterByName(0, "gColor");
	m_WorldMatHandle = m_Effect->GetParameterByName(0, "gWorldViewProjectMatrix");
	m_TechniqueHandle = m_Effect->GetTechniqueByName("ColorTech");
}

void ColorMaterial::PreRender(D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat)
{
	BaseMaterial::PreRender(worldMat, viewProjMat);

	m_WorldMat = worldMat * viewProjMat;

	HR(m_Effect->SetTechnique(m_TechniqueHandle));

	HR(m_Effect->SetMatrix(m_WorldMatHandle, &m_WorldMat));
	HR(m_Effect->SetValue(m_ColorHandle, &m_Color, sizeof(D3DXCOLOR)));
}

void ColorMaterial::setColor(FLOAT r, FLOAT g, FLOAT b, FLOAT a)
{
	m_Color = D3DXCOLOR(r, g, b, a);
}

void ColorMaterial::setColor(const D3DXCOLOR& color)
{
	m_Color = D3DXCOLOR(color.r, color.g, color.b, color.a);
}