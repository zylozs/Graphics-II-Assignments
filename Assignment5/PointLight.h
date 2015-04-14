#ifndef _POINT_LIGHT_H
#define _POINT_LIGHT_H

#include "3DClasses/BaseObject3D.h"

class PointLight : public BaseObject3D
{
private:
	D3DXVECTOR3 m_Attenuation;

public:
	PointLight(FLOAT attenX, FLOAT attenY, FLOAT attenZ);
	~PointLight();

	virtual void dispose();
	virtual void Create(IDirect3DDevice9* gd3dDevice);

	const D3DXVECTOR3& getAttenuation() { return m_Attenuation; }
	void setAttenuation(FLOAT x, FLOAT y, FLOAT z);
};

#endif