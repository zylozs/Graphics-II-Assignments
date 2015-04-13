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
#include "Camera.h"
#include "SkyBox.h"
#include <crtdbg.h>
#include "GfxStats.h"
#include <list>
#include "MemoryTracker.h"
#include "KeyEvent.h"
#include "Delegate.h"
#include "BaseMaterial.h"
#include "TextureMaterial.h"
#include "PhongTextureMaterial.h"

#include "SkeletonClass.h"
#include "3DClasses\BaseObject3D.h"
#include "3DClasses\Vertex.h"
#include "Cube.h"
#include "Cone.h"
#include "Cylinder.h"
#include "Sphere.h"
#include "Teapot.h"
#include "Torus.h"
#include "Planet.h"

#define STRENGTH_INCREMENT 0.1f

//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	g_EventDispatcher = New EventDispatcher();
	g_Camera = New Camera();

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

	g_Camera->dispose();
	delete g_Camera;
	g_Camera = NULL;

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

	m_SkyBox = NULL;

	g_Camera->setPosition(0.0f, 100.0f, 100.0f);
	//g_Camera->getPos().y = 0.0f;
	//g_Camera->getPos().z = -10.0f;
	g_Camera->setSpeed(10.0f);

	m_LightVec = D3DXVECTOR3(0.5f, 0.5f, 1.5f);

	m_RenderType = D3DFILL_SOLID;

	m_Texture = "Assets/fire.jpg";
	m_NormalMapTexture = "Assets/fire_nmap.png";

	m_Texture2 = "Assets/bricks_color.bmp";
	m_NormalMap2Texture = "Assets/bricks_nmap.bmp";

	m_Texture3 = "Assets/floor_color.bmp";
	m_NormalMap3Texture = "Assets/floor_nmap.bmp";

	m_EnvMapTexture = "Assets/spaceCubeMap.dds";

	m_TextureMaterialFX = "FX/PhongTexture.fx";
	m_IsWireframe = true;

	onResetDevice();

	InitAllVertexDeclarations();

	m_SkyBox = New SkyBox(m_EnvMapTexture, 10000.0f);

	// replace or add to the following object creation
	//m_Objects.push_back(New Sphere(5.0f, 50));
	//m_Objects.push_back(New Sphere(2.0f, 50));
	//m_Objects.push_back(New Sphere(1.0f, 50));
	m_Objects.push_back(New Planet(5.0f, 0.5f, 0.25f));
	m_Objects.push_back(New Planet(2.0f, 0.1f, 1.0f));
	m_Objects.push_back(New Planet(2.0f, 0.1f, 1.0f));
	m_Objects.push_back(New Planet(2.0f, 0.1f, 1.0f));
	m_Objects.push_back(New Planet(1.0f, 0.05f, 0.5f));
	m_Objects.push_back(New Planet(1.0f, 0.05f, 0.5f));

	for (UINT i = 0; i < m_Objects.size(); i++)
	{
		std::string texture = m_Texture;
		std::string normal = m_NormalMapTexture;

		PhongTextureMaterial* textureMaterial = New PhongTextureMaterial(texture, normal);
		textureMaterial->LoadEffectFromFile(gd3dDevice, m_TextureMaterialFX);
		textureMaterial->setActiveTechnique("Phong");

		m_Objects[i]->Create(gd3dDevice);
		m_Objects[i]->addMaterial("Texture", textureMaterial, true);
	}

	m_Objects[0]->addChild(g_Camera);
	m_Objects[0]->addChild(m_Objects[1]);
	m_Objects[0]->addChild(m_Objects[2]);
	m_Objects[0]->addChild(m_Objects[3]);
	m_Objects[1]->addChild(m_Objects[4]);
	m_Objects[1]->addChild(m_Objects[5]);

	m_Objects[1]->setPosition(30.0f, 0.0f, 0.0f);
	m_Objects[2]->setPosition(-60.0f, 0.0f, 0.0f);
	m_Objects[3]->setPosition(-25.0f, 0.0f, 25.0f);
	m_Objects[4]->setPosition(5.0f, 0.0f, 0.0f);
	m_Objects[5]->setPosition(2.5f, 0.0f, 2.5f);
}

SkeletonClass::~SkeletonClass()
{
    GfxStats::DeleteInstance();

	g_Input->removeEventListener(KEY_UP, m_KeyUpDelegate);
	
	for (unsigned int obj = 0; obj < m_Objects.size(); obj++)
	{
		m_Objects[obj]->dispose();
		delete m_Objects[obj];
		m_Objects[obj] = NULL;
	}
    m_Objects.clear();


	m_SkyBox->dispose();
	delete m_SkyBox;
	m_SkyBox = NULL;

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

	if (m_SkyBox != NULL)
		m_SkyBox->onLostDevice();
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
	float w = (float)md3dPP.BackBufferWidth;
	float h = (float)md3dPP.BackBufferHeight;
	g_Camera->setLens(D3DX_PI * 0.25f, w / h, 1.0f, 5000.0f);

	if (m_SkyBox != NULL)
		m_SkyBox->onResetDevice();
}

void SkeletonClass::onKeyUp(Event* ev)
{
	KeyEvent* key = (KeyEvent*)ev;

	switch (key->getKey())
	{
		case Keys::W:	//Wireframe/Not Wireframe
			swapRenderType();
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

	for (unsigned int i = 0; i < m_Objects.size(); i++)
	{
		m_Objects[i]->Update(dt);
	}

	if (g_Camera->isRoot())
		g_Camera->Update(dt);
}


void SkeletonClass::drawScene()
{
	// Clear the backbuffer and depth buffer.
	HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0));

	HR(gd3dDevice->BeginScene());

	m_SkyBox->draw();

	// Set render states for the entire scene here:
	HR(gd3dDevice->SetRenderState(D3DRS_FILLMODE, m_RenderType));

	GfxStats::GetInstance()->setShader(m_Objects[0]->getMaterial("Texture")->getActiveTechnique());
	GfxStats::GetInstance()->setFillMode(m_RenderType);

	D3DXMATRIX view = g_Camera->getView();
	D3DXMATRIX proj = g_Camera->getProj();

	// Render the specific object
	for (unsigned int i = 0; i < m_Objects.size(); i++)
	{
		m_Objects[i]->Render(gd3dDevice, view, proj, m_LightVec, g_Camera->getPos());
	}

    // display the render statistics
    GfxStats::GetInstance()->display();

	HR(gd3dDevice->EndScene());

	// Present the backbuffer.
	HR(gd3dDevice->Present(0, 0, 0, 0));
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