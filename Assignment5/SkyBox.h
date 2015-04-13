#ifndef _SKY_BOX_H
#define _SKY_BOX_H

#include <d3dx9.h>
#include <string>
#include "IObject.h"

class Sphere;

class SkyBox : public IObject
{
private:
	Sphere* m_Sphere;
	float m_Radius;

public:
	SkyBox(const std::string& envmapFilename, float skyRadius);
	~SkyBox();

	IDirect3DCubeTexture9* getEnvMap();
	float getRadius();

	void onLostDevice();
	void onResetDevice();

	void draw();

	virtual void dispose();
};

#endif // SKY_H