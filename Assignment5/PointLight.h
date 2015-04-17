#ifndef _POINT_LIGHT_H
#define _POINT_LIGHT_H

#include "Sphere.h"

class PointLight : public Sphere
{
private:
	D3DXVECTOR3 m_Attenuation;

public:
	PointLight(FLOAT attenX, FLOAT attenY, FLOAT attenZ);
	~PointLight();

	virtual void dispose();

	const D3DXVECTOR3& getAttenuation() { return m_Attenuation; }
	void setAttenuation(FLOAT x, FLOAT y, FLOAT z);
};

#endif