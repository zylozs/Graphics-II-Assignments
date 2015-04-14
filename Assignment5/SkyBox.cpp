#include "SkyBox.h"
#include "Camera.h"
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

void SkyBox::draw()
{
	D3DXMATRIX view = g_Camera->getView();
	D3DXMATRIX proj = g_Camera->getProj();
	D3DXVECTOR3 cameraPos = g_Camera->getPosition();
	m_Sphere->Render(gd3dDevice, view, proj, D3DXVECTOR3(), cameraPos);
}