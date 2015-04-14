#include "PointLight.h"

PointLight::PointLight(FLOAT attenX, FLOAT attenY, FLOAT attenZ)
{
	m_Attenuation = D3DXVECTOR3(attenX, attenY, attenZ);
}

PointLight::~PointLight()
{

}

void PointLight::dispose()
{

}

void PointLight::Create(IDirect3DDevice9* gd3dDevice)
{

}

void PointLight::setAttenuation(FLOAT x, FLOAT y, FLOAT z)
{
	m_Attenuation.x = x;
	m_Attenuation.y = y;
	m_Attenuation.z = z;
}