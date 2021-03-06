/*
*	Vincent Loignon
*	Assignment 4
*	SkyBox.cpp
*	I certify that this assignment is entirely our own work.
*/

#include "SkyBox.h"
#include "d3dUtil.h"
#include "CubeMapMaterial.h"
#include "Sphere.h"

SkyBox::SkyBox(const std::string& envmapFilename, float skyRadius)
	: m_Radius(skyRadius)
{
	m_Sphere = New Sphere(skyRadius, 30);
	CubeMapMaterial* mat = New CubeMapMaterial(envmapFilename);
	mat->LoadEffectFromFile(gd3dDevice, "FX/CubeMap.fx");

	m_Sphere->Create(gd3dDevice);
	m_Sphere->addMaterial("CubeMap", mat, true);
}

SkyBox::~SkyBox()
{
	
}

void SkyBox::dispose()
{
	m_Sphere->dispose();
	delete m_Sphere;
	m_Sphere = NULL;
}

IDirect3DCubeTexture9* SkyBox::getEnvMap()
{
	return ((CubeMapMaterial*)m_Sphere->getMaterial("CubeMap"))->getEnvMap();
}

float SkyBox::getRadius()
{
	return m_Radius;
}

void SkyBox::onLostDevice()
{
	m_Sphere->onLostDevice();
}

void SkyBox::onResetDevice()
{
	m_Sphere->onResetDevice();
}

void SkyBox::draw(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, D3DXVECTOR3& lightVec, D3DXVECTOR3& viewPos)
{
	m_Sphere->Render(gd3dDevice, view, projection, lightVec, viewPos);
}