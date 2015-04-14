#ifndef _PLANET_H
#define _PLANET_H

#include "3DClasses/BaseObject3D.h"
#include <string>

class Planet : public BaseObject3D
{
protected:
	BaseObject3D* m_PlanetObj; // This is the sphere object which visually represents this planet

	float m_RotationSpeed;
	float m_ChildRotationSpeed;

public:
	Planet(BaseObject3D* planetObj, FLOAT rotationSpeed, FLOAT childRotationSpeed, std::string name = "");
	~Planet();

	virtual void addMaterial(std::string key, BaseMaterial* material, bool setToActive = false);
	virtual void setActiveMaterial(std::string key);
	virtual void setUseMaterial(bool value);

	virtual BaseMaterial* getMaterial(std::string key);
	virtual bool getUseMaterial();

	virtual void Create(IDirect3DDevice9* gd3dDevice);
	virtual void Update(float dt);
	virtual void Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection, D3DXVECTOR3& lightVec, D3DXVECTOR3& viewPos);
	virtual void dispose();

	virtual void onLostDevice();
	virtual void onResetDevice();
};

#endif