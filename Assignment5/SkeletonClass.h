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
// Modified by Vincent Loignon and Tyler Cifelli
//
// Controls: Use mouse to orbit and zoom; Press P to switch between Gouraud and Phong
//			 shading. Press O (the letter) to switch between the different objects.
//			 Press T to toggle texturing. Press S to toggle the specular component.
//			 Press D to toggle the diffuse component. Press W to switch between solid
//			 and wireframe fill modes.
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
class SkyBox;
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

private: //functions
	void onKeyUp(Event* ev);
	void swapRenderType();
	void changeSelectedObject();

private: //variables
	D3DXVECTOR3 m_LightVec;

	std::string m_TextureMaterialFX;
	
	std::string m_Texture;
	std::string m_Texture2;
	std::string m_Texture3;

	std::string m_EnvMapTexture;
	std::string m_NormalMapTexture;
	std::string m_NormalMap2Texture;
	std::string m_NormalMap3Texture;

	SkyBox* m_SkyBox;

	DWORD m_RenderType; // D3DFILL_SOLID, D3DFILL_WIREFRAME
	bool m_IsWireframe;

	Delegate* m_KeyUpDelegate; // For Key Up events

    std::vector<BaseObject3D*> m_Objects;
	int m_ObjectIndex;

};
//=============================================================================
#endif // _SKELETON_CLASS_H_