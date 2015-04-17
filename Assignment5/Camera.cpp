//=============================================================================
// Camera.cpp by Frank Luna (C) 2004 All Rights Reserved.
//=============================================================================

#include "Camera.h"
#include "d3dUtil.h"
#include "InputSystem.h"

Camera* g_Camera = 0;

Camera::Camera()
{
	D3DXMatrixIdentity(&m_View);
	D3DXMatrixIdentity(&m_Proj);
	D3DXMatrixIdentity(&m_ViewProj);

	m_PosW   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_RightW = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_UpW    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_LookW  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	// Client should adjust to a value that makes sense for application's
	// unit scale, and the object the camera is attached to--e.g., car, jet,
	// human walking, etc.
	m_Speed  = 50.0f;
}

Camera::~Camera()
{

}

void Camera::dispose()
{
	BaseObject3D::dispose();
}

void Camera::lookAt(D3DXVECTOR3& pos, D3DXVECTOR3& target, D3DXVECTOR3& up)
{
	D3DXVECTOR3 L = target - pos;
	D3DXVec3Normalize(&L, &L);

	D3DXVECTOR3 R;
	D3DXVec3Cross(&R, &up, &L);
	D3DXVec3Normalize(&R, &R);

	D3DXVECTOR3 U;
	D3DXVec3Cross(&U, &L, &R);
	D3DXVec3Normalize(&U, &U);

	m_Position = pos;
	m_RightW = R;
	m_UpW = U;
	m_LookW = L;

	buildView();
	buildWorldFrustumPlanes();

	m_ViewProj = m_View * m_Proj;
}

void Camera::setLens(float fov, float aspect, float nearZ, float farZ)
{
	D3DXMatrixPerspectiveFovLH(&m_Proj, fov, aspect, nearZ, farZ);
	buildWorldFrustumPlanes();
	m_ViewProj = m_View * m_Proj;
}

void Camera::Create(IDirect3DDevice9* gd3dDevice)
{

}

/*bool Camera::isVisible(const AABB& box)const
{
	// Test assumes frustum planes face inward.

	D3DXVECTOR3 P;
	D3DXVECTOR3 Q;

	//      N  *Q                    *P
	//      | /                     /
	//      |/                     /
	// -----/----- Plane     -----/----- Plane    
	//     /                     / |
	//    /                     /  |
	//   *P                    *Q  N
	//
	// PQ forms diagonal most closely aligned with plane normal.

	// For each frustum plane, find the box diagonal (there are four main
	// diagonals that intersect the box center point) that points in the
	// same direction as the normal along each axis (i.e., the diagonal 
	// that is most aligned with the plane normal).  Then test if the box
	// is in front of the plane or not.
	for(int i = 0; i < 6; ++i)
	{
		// For each coordinate axis x, y, z...
		for(int j = 0; j < 3; ++j)
		{
			// Make PQ point in the same direction as the plane normal on this axis.
			if( m_FrustumPlanes[i][j] >= 0.0f )
			{
				P[j] = box.minPt[j];
				Q[j] = box.maxPt[j];
			}
			else 
			{
				P[j] = box.maxPt[j];
				Q[j] = box.minPt[j];
			}
		}

		// If box is in negative half space, it is behind the plane, and thus, completely
		// outside the frustum.  Note that because PQ points roughly in the direction of the 
		// plane normal, we can deduce that if Q is outside then P is also outside--thus we
		// only need to test Q.
		if( D3DXPlaneDotCoord(&m_FrustumPlanes[i], &Q) < 0.0f  ) // outside
			return false;
	}
	return true;
}*/

void Camera::Update(float dt)
{
	if (!isAttached())
	{
		// Find the net direction the camera is traveling in (since the
		// camera could be running and strafing).
		D3DXVECTOR3 dir(0.0f, 0.0f, 0.0f);
		if (g_Input->isKeyDown(Keys::W))
			dir += m_LookW;
		if (g_Input->isKeyDown(Keys::S))
			dir -= m_LookW;
		if (g_Input->isKeyDown(Keys::D))
			dir += m_RightW;
		if (g_Input->isKeyDown(Keys::A))
			dir -= m_RightW;

		// Move at mSpeed along net direction.
		D3DXVec3Normalize(&dir, &dir);
		D3DXVECTOR3 newPos = m_Position + dir * m_Speed * dt;

		m_Position = newPos;

		// We rotate at a fixed speed.
		float pitch = g_Input->getMouseDY() / 150.0f;
		float yAngle = g_Input->getMouseDX() / 150.0f;

		// Rotate camera's look and up vectors around the camera's right vector.
		D3DXMATRIX R;
		D3DXMatrixRotationAxis(&R, &m_RightW, pitch);
		D3DXVec3TransformCoord(&m_LookW, &m_LookW, &R);
		D3DXVec3TransformCoord(&m_UpW, &m_UpW, &R);

		// Rotate camera axes about the world's y-axis.
		D3DXMatrixRotationY(&R, yAngle);
		D3DXVec3TransformCoord(&m_RightW, &m_RightW, &R);
		D3DXVec3TransformCoord(&m_UpW, &m_UpW, &R);
		D3DXVec3TransformCoord(&m_LookW, &m_LookW, &R);

		// Rebuild the view matrix to reflect changes.
		buildView();
		buildWorldFrustumPlanes();

		m_ViewProj = m_View * m_Proj;
	}
	else
	{
		D3DXVECTOR3 parentPos;
		D3DXVECTOR3 scale;
		D3DXQUATERNION rot;
		D3DXMATRIX parentWorldMatrix = m_Parent->getWorldMatrix();

		D3DXMatrixDecompose(&scale, &rot, &parentPos, &parentWorldMatrix);

		float moveAmount = g_Input->getMouseDZ() / 75.0f;

		D3DXVECTOR3 direction = parentPos - m_Position;
		D3DXVec3Normalize(&direction, &direction);
		direction *= moveAmount;

		addToPosition(direction.x, direction.y, direction.z);

		calculateWorldMatrix();

		lookAt(m_Position, parentPos, D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	}
}

void Camera::buildView()
{
	// Keep camera's axes orthogonal to each other and of unit length.
	D3DXVec3Normalize(&m_LookW, &m_LookW);

	D3DXVec3Cross(&m_UpW, &m_LookW, &m_RightW);
	D3DXVec3Normalize(&m_UpW, &m_UpW);

	D3DXVec3Cross(&m_RightW, &m_UpW, &m_LookW);
	D3DXVec3Normalize(&m_RightW, &m_RightW);

	// Fill in the view matrix entries.

	float x = -D3DXVec3Dot(&m_Position, &m_RightW);
	float y = -D3DXVec3Dot(&m_Position, &m_UpW);
	float z = -D3DXVec3Dot(&m_Position, &m_LookW);

	m_View(0,0) = m_RightW.x; 
	m_View(1,0) = m_RightW.y; 
	m_View(2,0) = m_RightW.z; 
	m_View(3,0) = x;   

	m_View(0,1) = m_UpW.x;
	m_View(1,1) = m_UpW.y;
	m_View(2,1) = m_UpW.z;
	m_View(3,1) = y;  

	m_View(0, 2) = m_LookW.x;
	m_View(1, 2) = m_LookW.y;
	m_View(2, 2) = m_LookW.z;
	m_View(3,2) = z;   

	m_View(0,3) = 0.0f;
	m_View(1,3) = 0.0f;
	m_View(2,3) = 0.0f;
	m_View(3,3) = 1.0f;
}

void Camera::buildWorldFrustumPlanes()
{
	// Note: Extract the frustum planes in world space.

	D3DXMATRIX VP = m_View * m_Proj;

	D3DXVECTOR4 col0(VP(0,0), VP(1,0), VP(2,0), VP(3,0));
	D3DXVECTOR4 col1(VP(0,1), VP(1,1), VP(2,1), VP(3,1));
	D3DXVECTOR4 col2(VP(0,2), VP(1,2), VP(2,2), VP(3,2));
	D3DXVECTOR4 col3(VP(0,3), VP(1,3), VP(2,3), VP(3,3));

	// Planes face inward.
	m_FrustumPlanes[0] = (D3DXPLANE)(col2);        // near
	m_FrustumPlanes[1] = (D3DXPLANE)(col3 - col2); // far
	m_FrustumPlanes[2] = (D3DXPLANE)(col3 + col0); // left
	m_FrustumPlanes[3] = (D3DXPLANE)(col3 - col0); // right
	m_FrustumPlanes[4] = (D3DXPLANE)(col3 - col1); // top
	m_FrustumPlanes[5] = (D3DXPLANE)(col3 + col1); // bottom

	for(int i = 0; i < 6; i++)
		D3DXPlaneNormalize(&m_FrustumPlanes[i], &m_FrustumPlanes[i]);
}