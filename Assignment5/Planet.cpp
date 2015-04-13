#include "Planet.h"
#include "Sphere.h"

Planet::Planet(float radius, float rotationSpeed, float childRotationSpeed)
{
	m_Sphere = New Sphere(radius, 50);
	m_Sphere->setPosition(0, 0, 0);
	this->addChild(m_Sphere);

	m_RotationSpeed = rotationSpeed;
	m_ChildRotationSpeed = childRotationSpeed;
}

Planet::~Planet()
{
	
}

void Planet::dispose()
{
	BaseObject3D::dispose();

	m_Sphere->dispose();
	delete m_Sphere;
	m_Sphere = NULL;
}

void Planet::addMaterial(std::string key, BaseMaterial* material, bool setToActive /* = false */)
{
	m_Sphere->addMaterial(key, material, setToActive);
}

void Planet::setActiveMaterial(std::string key)
{
	m_Sphere->setActiveMaterial(key);
}

void Planet::setUseMaterial(bool value)
{
	m_Sphere->setUseMaterial(value);
}

BaseMaterial* Planet::getMaterial(std::string key)
{
	return m_Sphere->getMaterial(key);
}

bool Planet::getUseMaterial()
{
	return m_Sphere->getUseMaterial();
}

void Planet::Create(IDirect3DDevice9* gd3dDevice)
{
	m_Sphere->Create(gd3dDevice);
}

void Planet::Update(float dt)
{
	m_Sphere->addToRotation(m_RotationSpeed * dt, 0.0f, 0.0f);
	this->addToRotation(m_ChildRotationSpeed * dt, 0.0f, 0.0f);

	BaseObject3D::Update(dt);
}

void Planet::Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, D3DXVECTOR3& lightVec, D3DXVECTOR3& viewPos)
{
	m_Sphere->Render(gd3dDevice, view, projection, lightVec, viewPos);
}

void Planet::onLostDevice()
{
	BaseObject3D::onLostDevice();

	m_Sphere->onLostDevice();
}

void Planet::onResetDevice()
{
	BaseObject3D::onResetDevice();

	m_Sphere->onResetDevice();
}