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

	SkeletonClass app(hInstance, "Exercise Skeleton Project", D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING);
	gd3dApp = &app;

	g_Input = New InputSystem(true, false);
	g_Input->initialize(hInstance, gd3dApp->getMainWnd());
	g_Input->disableEvents(InputEventType::KEY_DOWN);

	int returnVal = gd3dApp->run();

	g_EventDispatcher->dispose();
	delete g_EventDispatcher;
	g_EventDispatcher = NULL;

	g_Input->dispose();
	delete g_Input;
	g_Input = NULL;

	g_MemoryTracker.reportAllocations(std::cout);

	system("pause");
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

	mCameraRadius    = 10.0f;
	mCameraRotationY = 1.2 * D3DX_PI;
	mCameraHeight    = 5.0f;

    // repleace or add to the following object creation
	m_Objects.push_back(new Cube(1.0f, 1.0f, 1.0f));
	m_Objects.push_back(new Sphere(1.0f, 20));
	m_Objects.push_back(new Cylinder(1.0f, 2.0f, 20));
	m_Objects.push_back(new Cone(1.0f, 2.0f, 20));

	for (UINT i = 0; i < m_Objects.size(); i++)
	{
		m_Objects[i]->Create(gd3dDevice);
	}

	m_Objects[0]->setCenterPos(-3.0f, 0.0f, 0.0f);
	m_Objects[1]->setCenterPos(0.0f, 3.0f, 0.0f);
	m_Objects[2]->setCenterPos(3.0f, 0.0f, 0.0f);

	onResetDevice();

	InitAllVertexDeclarations();
}

SkeletonClass::~SkeletonClass()
{
    GfxStats::DeleteInstance();

    for ( unsigned int obj=0 ; obj<m_Objects.size() ; obj++ )
        delete m_Objects[obj];
    m_Objects.clear();

	DestroyAllVertexDeclarations();
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

	// Check input.
	if (g_Input->isKeyDown(Keys::W))	 
		mCameraHeight   += 25.0f * dt;
	if (g_Input->isKeyDown(Keys::S))
		mCameraHeight   -= 25.0f * dt;

	// Divide by 50 to make mouse less sensitive. 
	mCameraRotationY += g_Input->getMouseDX() / 100.0f;
	mCameraRadius    += -g_Input->getMouseDZ() / 50.0f;

	// If we rotate over 360 degrees, just roll back to 0
	if( fabsf(mCameraRotationY) >= 2.0f * D3DX_PI ) 
		mCameraRotationY = 0.0f;

	// Don't let radius get too small.
	if( mCameraRadius < 5.0f )
		mCameraRadius = 5.0f;

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

    // Set render statws for the entire scene here:
//	HR(gd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID));
	HR(gd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME));

    // Render all the objects
    for ( unsigned int obj=0 ; obj<m_Objects.size() ; obj++ )
    {
        m_Objects[obj]->Render( gd3dDevice, mView, mProj );
    }

    // display the render statistics
    GfxStats::GetInstance()->display();

	HR(gd3dDevice->EndScene());

	// Present the backbuffer.
	HR(gd3dDevice->Present(0, 0, 0, 0));
}

void SkeletonClass::buildViewMtx()
{
	float x = mCameraRadius * cosf(mCameraRotationY);
	float z = mCameraRadius * sinf(mCameraRotationY);
	D3DXVECTOR3 pos(x, mCameraHeight, z);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&mView, &pos, &target, &up);
}

void SkeletonClass::buildProjMtx()
{
	float w = (float)md3dPP.BackBufferWidth;
	float h = (float)md3dPP.BackBufferHeight;
	D3DXMatrixPerspectiveFovLH(&mProj, D3DX_PI * 0.25f, w/h, 1.0f, 5000.0f);
}