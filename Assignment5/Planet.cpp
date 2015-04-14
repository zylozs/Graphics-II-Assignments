#include "Planet.h"
#include "Sphere.h"

Planet::Planet(BaseObject3D* planetObj, float rotationSpeed, float childRotationSpeed, std::string name /* = "" */)
{
	if (name != "")
		m_Name = name;

	m_PlanetObj = planetObj;
	m_PlanetObj->setPosition(0, 0, 0);
	this->addChild(m_PlanetObj);

	m_RotationSpeed = rotationSpeed;
	m_ChildRotationSpeed = childRotationSpeed;
}

Planet::~Planet()
{
	
}

void Planet::dispose()
{
	BaseObject3D::dispose();

	m_PlanetObj->dispose();
	delete m_PlanetObj;
	m_PlanetObj = NULL;
}

void Planet::addMaterial(std::string key, BaseMaterial* material, bool setToActive /* = false */)
{
	m_PlanetObj->addMaterial(key, material, setToActive);
}

void Planet::setActiveMaterial(std::string key)
{
	m_PlanetObj->setActiveMaterial(key);
}

void Planet::setUseMaterial(bool value)
{
	m_PlanetObj->setUseMaterial(value);
}

BaseMaterial* Planet::getMaterial(std::string key)
{
	return m_PlanetObj->getMaterial(key);
}

bool Planet::getUseMaterial()
{
	return m_PlanetObj->getUseMaterial();
}

void Planet::Create(IDirect3DDevice9* gd3dDevice)
{
	m_PlanetObj->Create(gd3dDevice);
}

void Planet::Update(float dt)
{
	m_PlanetObj->addToRotation(m_RotationSpeed * dt, 0.0f, 0.0f);
	this->addToRotation(m_ChildRotationSpeed * dt, 0.0f, 0.0f);

	BaseObject3D::Update(dt);
}

void Planet::Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, D3DXVECTOR3& lightVec, D3DXVECTOR3& viewPos)
{
	//BaseObject3D::Render(gd3dDevice, view, projection, lightVec, viewPos);

	/*D3DXMATRIX localLight;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 scale;
	D3DXQUATERNION rot;
	D3DXMatrixTranslation(&localLight, lightVec.x, lightVec.y, lightVec.z);
	D3DXMatrixMultiply(&localLight, &m_World, &localLight);
	D3DXMatrixDecompose(&scale, &rot, &pos, &localLight);*/

	m_PlanetObj->Render(gd3dDevice, view, projection, lightVec, viewPos);
}

void Planet::onLostDevice()
{
	BaseObject3D::onLostDevice();

	m_PlanetObj->onLostDevice();
}

void Planet::onResetDevice()
{
	BaseObject3D::onResetDevice();

	m_PlanetObj->onResetDevice();
}