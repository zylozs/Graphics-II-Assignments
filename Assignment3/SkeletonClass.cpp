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
// Controls: Use mouse to orbit and zoom; use the 'W' and 'S' keys to 
//           alter the height of the camera.
//=============================================================================
#include "InputSystem.h"
#include <crtdbg.h>
#include "GfxStats.h"
#include <list>
#include "MemoryTracker.h"
#include "KeyEvent.h"
#include "Delegate.h"
#include "BaseMaterial.h"
#include "ColorMaterial.h"

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

	m_RenderType = D3DFILL_WIREFRAME;

    // repleace or add to the following object creation
	m_Objects.push_back(New Cube(1.0f, 1.0f, 1.0f));
	m_Objects.push_back(New Sphere(1.0f, 20));
	m_Objects.push_back(New Cylinder(1.0f, 2, 20));
	m_Objects.push_back(New Cone(1.0f, 2, 20));

	for (UINT i = 0; i < m_Objects.size(); i++)
	{
		BaseMaterial* material = New ColorMaterial(1.0f, 1.0f, 0.0f, 1.0f);
		material->LoadEffectFromFile(gd3dDevice, "FX/Color.fx");

		m_Objects[i]->Create(gd3dDevice);
		m_Objects[i]->setMaterial(material);
	}

	m_ObjectIndex = 0;

	onResetDevice();

	InitAllVertexDeclarations();
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
}

void SkeletonClass::onResetDevice()
{
	GfxStats::GetInstance()->onResetDevice();

	// The aspect ratio depends on the backbuffer dimensions, which can 
	// possibly change after a reset.  So rebuild the projection matrix.
	buildProjMtx();
}

void SkeletonClass::onKeyUp(Event* ev)
{
	KeyEvent* key = (KeyEvent*)ev;

	switch (key->getKey())
	{
		case Keys::W:
			swapRenderType();
			break;
		case Keys::O:
			changeSelectedObject();
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
	m_CameraRotationX += g_Input->getMouseDY() / 50.0f;

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

    // Render the specific object
    m_Objects[m_ObjectIndex]->Render( gd3dDevice, mView, mProj );

    // display the render statistics
    GfxStats::GetInstance()->display();

	HR(gd3dDevice->EndScene());

	// Present the backbuffer.
	HR(gd3dDevice->Present(0, 0, 0, 0));
}

void SkeletonClass::buildViewMtx()
{
	// Bad way
// 	float x = mCameraRadius * cosf(mCameraRotationY);
// 	float z = mCameraRadius * sinf(mCameraRotationY);
// 	D3DXVECTOR3 pos(x, mCameraHeight, z);
// 	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
// 	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
// 	D3DXMatrixLookAtLH(&mView, &pos, &target, &up);

	// Good way
	D3DXMATRIX move;
	D3DXMATRIX upDownRotate;
	D3DXMATRIX leftRightRotate;

	D3DXMatrixTranslation(&move, 0, 0, -m_CameraRadius); // -Z axis using camera radius
	D3DXMatrixRotationX(&upDownRotate, m_CameraRotationX); // Up Down Rotation
	D3DXMatrixRotationY(&leftRightRotate, m_CameraRotationY); // left right rotation

	D3DXMatrixMultiply(&mView, &move, &upDownRotate);
	D3DXMatrixMultiply(&mView, &mView, &leftRightRotate);

	D3DXMatrixInverse(&mView, NULL, &mView); // Invert
}

void SkeletonClass::buildProjMtx()
{
	float w = (float)md3dPP.BackBufferWidth;
	float h = (float)md3dPP.BackBufferHeight;
	D3DXMatrixPerspectiveFovLH(&mProj, D3DX_PI * 0.25f, w/h, 1.0f, 5000.0f);
}

void SkeletonClass::swapRenderType()
{
	if (m_RenderType == D3DFILL_WIREFRAME)
		m_RenderType = D3DFILL_SOLID;
	else if (m_RenderType == D3DFILL_SOLID)
		m_RenderType = D3DFILL_WIREFRAME;
}

void SkeletonClass::changeSelectedObject()
{
	m_ObjectIndex++;

	if (m_ObjectIndex >= (int)m_Objects.size())
		m_ObjectIndex = 0;
}