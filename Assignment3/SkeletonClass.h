//=============================================================================
//                          SkeletonClass.h
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
#ifndef _SKELETON_CLASS_H
#define _SKELETON_CLASS_H
//=============================================================================
#pragma once
//=============================================================================
#include "d3dApp.h"

#include <vector>
//=============================================================================
class BaseObject3D;
class InputSystem;
//=============================================================================
class SkeletonClass : public D3DApp
{
public:
	SkeletonClass(HINSTANCE hInstance, std::string winCaption, D3DDEVTYPE devType, DWORD requestedVP);
	~SkeletonClass();

	virtual void initialize();
	virtual bool checkDeviceCaps();
	virtual void onLostDevice();
	virtual void onResetDevice();
	virtual void updateScene(float dt);
	virtual void drawScene();

	// Helper methods
	void buildViewMtx();
	void buildProjMtx();

private:
	void onKeyUp(Event* ev);
	void swapRenderType();

private:
	float mCameraRotationY;
	float mCameraRadius;
	float mCameraHeight;

	D3DXMATRIX mView;
	D3DXMATRIX mProj;

	DWORD m_RenderType; // D3DFILL_SOLID, D3DFILL_WIREFRAME

	Delegate* m_KeyUpDelegate; // For Key Up events

    std::vector<BaseObject3D*>      m_Objects;
};
//=============================================================================
#endif // _SKELETON_CLASS_H_