#ifndef _COLOR_MATERIAL_H
#define _COLOR_MATERIAL_H

#include "BaseMaterial.h"

class ColorMaterial : public BaseMaterial
{
protected:
	D3DXCOLOR m_Color;

	D3DXHANDLE m_ColorHandle;

public:
	ColorMaterial(FLOAT r, FLOAT g, FLOAT b, FLOAT a);
	ColorMaterial(const D3DXCOLOR& color);
	~ColorMaterial();

	void setColor(FLOAT r, FLOAT g, FLOAT b, FLOAT a);
	void setColor(const D3DXCOLOR& color);

	virtual void ConnectToEffect(ID3DXEffect* effect);
	virtual void PreRender(D3DXMATRIX& worldMat, D3DXMATRIX& viewProjMat, D3DXVECTOR3& lightPos, D3DXVECTOR3& viewPos);
};

#endif