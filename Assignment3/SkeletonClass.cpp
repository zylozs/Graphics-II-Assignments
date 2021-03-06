//=============================================================================
//                          SkeletonClass.cpp
//
// Adopted from CubeDemo.cpp by Frank Luna (C) 2005 All Rights Reserved.
//
// Edited and changed by Adi Bar-Lev, 2013
// EGP-300-101 - Computer Graphics II, Spring 2013.
//
// This project creates a skeleton workspace and base classes for the student to be 
// able to start working with sample classes and develop evolve it according to the 
// given exercises
//
// Modified by Vincent Loignon and Tyler Cifelli
//
// Controls: Use mouse to orbit and zoom; Press P to switch between Gouraud and Phong
//			 shading. Press O (the letter) to switch between the different objects.
//			 Press T to toggle texturing. Press S to toggle the specular component.
//			 Press D to toggle the diffuse component. Press W to switch between solid
//			 and wireframe fill modes.
//=============================================================================
#include "InputSystem.h"
#include <crtdbg.h>
#include "GfxStats.h"
#include <list>
#include "MemoryTracker.h"
#include "KeyEvent.h"
#include "Delegate.h"
#include "BaseMaterial.h"
#include "TextureMaterial.h"

#include "SkeletonClass.h"
#include "3DClasses\BaseObject3D.h"
#include "3DClasses\Vertex.h"
#include "Cube.h"
#include "Cone.h"
#include "Cylinder.h"
#include "Sphere.h"
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	g_EventDispatcher = New EventDispatcher();

	gd3dApp = new SkeletonClass(hInstance, "Exercise Skeleton Project", D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING);

	g_Input = New InputSystem(true, false);
	g_Input->initialize(hInstance, gd3dApp->getMainWnd());
	g_Input->disableEvents(InputEventType::KEY_DOWN);

	gd3dApp->initialize();

	int returnVal = gd3dApp->run();
	
	delete gd3dApp;
	gd3dApp = NULL;

	g_EventDispatcher->dispose();
	delete g_EventDispatcher;
	g_EventDispatcher = NULL;

	g_Input->dispose();
	delete g_Input;
	g_Input = NULL;

	// Do this only in debug mode!
	#ifdef _DEBUG
		g_MemoryTracker.reportAllocations(std::cout);

		system("pause");
	#endif

	return returnVal;
}

SkeletonClass::SkeletonClass(HINSTANCE hInstance, std::string winCaption, D3DDEVTYPE devType, DWORD requestedVP)
: D3DApp(hInstance, winCaption, devType, requestedVP)
{
	if(!checkDeviceCaps())
	{
		MessageBox(0, "checkDeviceCaps() Failed", 0, 0);
		PostQuitMessage(0);
	}

	m_CameraRadius    = 10.0f;
	m_CameraRotationY = 1.2 * D3DX_PI;
	m_CameraRotationX    = 5.0f;

	m_LightVec = D3DXVECTOR3(0.5f, 0.5f, 1.5f);

	m_RenderType = D3DFILL_SOLID;
	m_ObjectColor = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_UseTexture = false;

	m_CubeTexture = "Assets/crate.jpg";
	//m_CubeTexture = "Assets/companion cube.png";
	m_SphereTexture = "Assets/rock.jpg";
	//m_CylinderTexture = "Assets/WhiteVermontMarble.jpg";
	m_CylinderTexture = "Assets/crate.jpg";
	m_ConeTexture = "Assets/gold-texture.jpg";
	//m_CylinderTexture = "Assets/rock.jpg";

	m_TextureMaterialFX = "FX/TextureMap.fx";
	m_ColorMaterialFX = "FX/Color.fx";
	m_IsWireframe = true;
	m_UseSpecular = true;
	m_UseDiffuse = true;

	onResetDevice();

	InitAllVertexDeclarations();

	// replace or add to the following object creation
	m_Objects.push_back(New Cube(1.0f, 1.0f, 1.0f));
	m_Objects.push_back(New Sphere(1.0f, 20));
	m_Objects.push_back(New Cylinder(1.0f, 2, 20));
	m_Objects.push_back(New Cone(1.0f, 2, 20));

	for (UINT i = 0; i < m_Objects.size(); i++)
	{
		std::string texture = m_CubeTexture;

		switch (i)
		{
			case 1:
				texture = m_SphereTexture;
				break;
			case 2:
				texture = m_CylinderTexture;
				break;
			case 3:
				texture = m_ConeTexture;
				break;
		}

		BaseMaterial* textureMaterial = New TextureMaterial(texture);
		textureMaterial->LoadEffectFromFile(gd3dDevice, m_TextureMaterialFX);

		//textureMaterial->setActiveTechnique("Phong");
		textureMaterial->setActiveTechnique("Gouraud");
		((TextureMaterial*)textureMaterial)->setUseTexture(m_UseTexture);

		m_Objects[i]->Create(gd3dDevice);
		m_Objects[i]->addMaterial("Texture", textureMaterial, true);
	}

	m_ObjectIndex = 0;
}

SkeletonClass::~SkeletonClass()
{
    GfxStats::DeleteInstance();

	g_Input->removeEventListener(KEY_UP, m_KeyUpDelegate);
	
    for ( unsigned int obj=0 ; obj<m_Objects.size() ; obj++ )
        delete m_Objects[obj];
    m_Objects.clear();

	DestroyAllVertexDeclarations();
}

void SkeletonClass::initialize()
{
	m_KeyUpDelegate = Delegate::create<SkeletonClass, &SkeletonClass::onKeyUp>(this);
	g_Input->addEventListener(KEY_UP, m_KeyUpDelegate);
}

bool SkeletonClass::checkDeviceCaps()
{
	return true;
}

void SkeletonClass::onLostDevice()
{
	GfxStats::GetInstance()->onLostDevice();

	for (UINT i = 0; i < m_Objects.size(); i++)
	{
		m_Objects[i]->onLostDevice();
	}
}

void SkeletonClass::onResetDevice()
{
	GfxStats::GetInstance()->onResetDevice();

	for (UINT i = 0; i < m_Objects.size(); i++)
	{
		m_Objects[i]->onResetDevice();
	}

	// The aspect ratio depends on the backbuffer dimensions, which can 
	// possibly change after a reset.  So rebuild the projection matrix.
	buildProjMtx();
}

void SkeletonClass::onKeyUp(Event* ev)
{
	KeyEvent* key = (KeyEvent*)ev;

	switch (key->getKey())
	{
		case Keys::W:	//Wireframe/Not Wireframe
			swapRenderType();
			break;
		case Keys::O:	//Object Swap
			changeSelectedObject();
			break;
		case Keys::S:	//Specular component swap
			toggleSpecularComponent();
			break;
		case Keys::D:	//Diffuse component swap
			toggleDiffuseComponent();
			break;
		case Keys::T:	//Texture/No Texture
			toggleTexture();
			break;
		case Keys::P:
			swapShaderTechnique();
			break;
	}
}

void SkeletonClass::updateScene(float dt)
{
	// Reset the statistics for the scene - each object will add to it.
	GfxStats::GetInstance()->setVertexCount(0);
	GfxStats::GetInstance()->setTriCount(0);
	GfxStats::GetInstance()->update(dt);

	// Get snapshot of input devices.
	g_Input->update();

	// Closes the Program when Escape is hit
	if (g_Input->isKeyDown(Keys::ESCAPE))
	{
		PostQuitMessage(0);
	}

	// Divide by 50 to make mouse less sensitive. 
	m_CameraRotationY += g_Input->getMouseDX() / 100.0f;
	m_CameraRadius    += -g_Input->getMouseDZ() / 50.0f;
	m_CameraRotationX += g_Input->getMouseDY() / 25.0f;

	// If we rotate over 360 degrees, just roll back to 0
	if( fabsf(m_CameraRotationY) >= 2.0f * D3DX_PI ) 
		m_CameraRotationY = 0.0f;

	// Don't let radius get too small.
	if( m_CameraRadius < 5.0f )
		m_CameraRadius = 5.0f;

	// The camera position/orientation relative to world space can 
	// change every frame based on input, so we need to rebuild the
	// view matrix every frame with the latest changes.
	buildViewMtx();
}


void SkeletonClass::drawScene()
{
	// Clear the backbuffer and depth buffer.
	HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0));

	HR(gd3dDevice->BeginScene());

    // Set render states for the entire scene here:
	HR(gd3dDevice->SetRenderState(D3DRS_FILLMODE, m_RenderType));

	GfxStats::GetInstance()->setShader(m_Objects[m_ObjectIndex]->getMaterial("Texture")->getActiveTechnique());
	GfxStats::GetInstance()->setFillMode(m_RenderType);

    // Render the specific object
    m_Objects[m_ObjectIndex]->Render( gd3dDevice, m_View, m_Proj, m_LightVec, m_ViewPos);

    // display the render statistics
    GfxStats::GetInstance()->display();

	HR(gd3dDevice->EndScene());

	// Present the backbuffer.
	HR(gd3dDevice->Present(0, 0, 0, 0));
}

void SkeletonClass::buildViewMtx()
{
	float x = m_CameraRadius * cosf(m_CameraRotationY);
	float z = m_CameraRadius * sinf(m_CameraRotationY);

	m_ViewPos = D3DXVECTOR3(x, m_CameraRotationX, z);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_View, &m_ViewPos, &target, &up);
}

void SkeletonClass::buildProjMtx()
{
	float w = (float)md3dPP.BackBufferWidth;
	float h = (float)md3dPP.BackBufferHeight;
	D3DXMatrixPerspectiveFovLH(&m_Proj, D3DX_PI * 0.25f, w/h, 1.0f, 5000.0f);
}

void SkeletonClass::swapRenderType()
{
	m_IsWireframe = !m_IsWireframe;

	if (m_RenderType == D3DFILL_WIREFRAME)
		m_RenderType = D3DFILL_SOLID;
	else if (m_RenderType == D3DFILL_SOLID)
		m_RenderType = D3DFILL_WIREFRAME;

	for (UINT i = 0; i < m_Objects.size(); i++)
	{
		m_Objects[i]->setUseMaterial(m_IsWireframe);
	}
}

void SkeletonClass::changeSelectedObject()
{
	m_ObjectIndex++;

	if (m_ObjectIndex >= (int)m_Objects.size())
		m_ObjectIndex = 0;
}

void SkeletonClass::toggleTexture()
{
	m_UseTexture = !m_UseTexture;

	for (UINT i = 0; i < m_Objects.size(); i++)
	{
		((TextureMaterial*)m_Objects[i]->getMaterial("Texture"))->setUseTexture(m_UseTexture);
	}
}

void SkeletonClass::toggleSpecularComponent()
{
	m_UseSpecular = !m_UseSpecular;

	for (UINT i = 0; i < m_Objects.size(); i++)
	{
		m_Objects[i]->getMaterial("Texture")->setUseSpecular(m_UseSpecular);
	}
}

void SkeletonClass::toggleDiffuseComponent()
{
	m_UseDiffuse = !m_UseDiffuse;

	for (UINT i = 0; i < m_Objects.size(); i++)
	{
		m_Objects[i]->getMaterial("Texture")->setUseDiffuse(m_UseDiffuse);
	}
}

void SkeletonClass::swapShaderTechnique()
{
	for (UINT i = 0; i < m_Objects.size(); i++)
	{
		BaseMaterial* mat = m_Objects[i]->getMaterial("Texture");

		if (mat->getActiveTechnique() == "Gouraud")
			mat->setActiveTechnique("Phong");
		else if (mat->getActiveTechnique() == "Phong")
			mat->setActiveTechnique("Gouraud");
	}
}