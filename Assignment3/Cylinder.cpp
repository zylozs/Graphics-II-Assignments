#include "Cylinder.h"
#include "3DClasses\Vertex.h"

Cylinder::Cylinder(float radius, UINT height, UINT sideFacetsNum)
	: m_Radius1(radius), m_Radius2(radius),
	m_Height((float)height), m_SideFacetsNum(sideFacetsNum)
{
	m_StackHeight = (UINT)m_Height / m_SideFacetsNum;

	//calculate radius step per stack level
	m_RadiusStep = (m_Radius1 - m_Radius2) / m_SideFacetsNum;
	
}

Cylinder::~Cylinder()
{

}

void Cylinder::Create(IDirect3DDevice9* gd3dDevice)
{
	D3DXCreateCylinder(gd3dDevice, m_Radius1, m_Radius2, m_Height, m_SideFacetsNum, m_SideFacetsNum, &m_Mesh, mp_Buffer);

	generateUVs();
}

void Cylinder::generateUVs()
{
	// Grab our vertex description
	D3DVERTEXELEMENT9 elements[64];
	UINT numElements = 0;
	Vertex::Decl->GetDeclaration(elements, &numElements);

	// Create a copy of our sphere mesh using our vertex description instead of the old one
	ID3DXMesh* temp = 0;
	HR(m_Mesh->CloneMesh(D3DXMESH_SYSTEMMEM, elements, gd3dDevice, &temp));

	// Release our old mesh since we have a copy we will be modifying in system memory
	ReleaseCOM(m_Mesh);

	// Lock the vertex buffer for modification
	Vertex* vertices = 0;
	HR(temp->LockVertexBuffer(0, (void**)&vertices));

	D3DXVECTOR3 maxPoint(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	D3DXVECTOR3 minPoint(FLT_MAX, FLT_MAX, FLT_MAX);

	for (UINT i = 0; i < temp->GetNumVertices(); ++i)
	{
		D3DXVec3Maximize(&maxPoint, &maxPoint, &vertices[i].Position);
		D3DXVec3Minimize(&minPoint, &minPoint, &vertices[i].Position);
	}

	float a = minPoint.z;
	float b = maxPoint.z;
	float h = b - a;

	// Iterate over each vertex and compute its texture coordinate.
	for (UINT i = 0; i < temp->GetNumVertices(); ++i)
	{
		// Get the coordinates along the axes orthogonal to the
		// axis with which the cylinder is aligned.

		float x = vertices[i].Position.x;
		float y = vertices[i].Position.z;
		float z = vertices[i].Position.y;

		// Convert to cylindrical coordinates.

		float theta = atan2f(z, x);
		float y2 = y - b; // Transform [a, b]-->[-h, 0]

		// Transform theta from [0, 2*pi] to [0, 1] range and
		// transform y2 from [-h, 0] to [0, 1].

		float u = theta / (2.0f*D3DX_PI);
		float v = y2 / -h;

		// Save texture coordinates.
		vertices[i].TexCoord.x = u;
		vertices[i].TexCoord.y = v;
	}

	// Unlock the buffer since we no longer need it
	HR(temp->UnlockVertexBuffer());

	// Clone the copy of the mesh back into the member variable with hardware friendly tags
	HR(temp->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY, elements, gd3dDevice, &m_Mesh));

	// Release our local copy since we no longer need it
	ReleaseCOM(temp);
}

/*
 // D3DXCreate* functions generate vertices with position
 // and normal data. But for texturing, we also need
 // tex-coords. So clone the mesh to change the vertex
 // format to a format with tex-coords.

 D3DVERTEXELEMENT9 elements[64];
 UINT numElements = 0;
 VertexPNT::Decl->GetDeclaration(elements, &numElements);

 ID3DXMesh* temp = 0;
 HR(mCylinder->CloneMesh(D3DXMESH_SYSTEMMEM,
 elements, gd3dDevice, &temp));

 ReleaseCOM(mCylinder);

 // Now generate texture coordinates for each vertex.
 VertexPNT* vertices = 0;
 HR(temp->LockVertexBuffer(0, (void**)&vertices));

 // We need to get the height of the cylinder onto which we are
 // projecting the vertices. That height depends on the axis on
 // which the client has specified that the cylinder lies. The
 // height is determined by finding the height of the bounding
 // cylinder on the specified axis.

 D3DXVECTOR3 maxPoint(-FLT_MAX, -FLT_MAX, -FLT_MAX);
 D3DXVECTOR3 minPoint(FLT_MAX, FLT_MAX, FLT_MAX);

 for(UINT i = 0; i < temp->GetNumVertices(); ++i)
 {

 D3DXVec3Maximize(&maxPoint, &maxPoint, &vertices[i].pos);
 D3DXVec3Minimize(&minPoint, &minPoint, &vertices[i].pos);
 }

 float a = 0.0f;
 float b = 0.0f;
 float h = 0.0f;
 switch( axis )
 {
 case X_AXIS:
 a = minPoint.x;
 b = maxPoint.x;
 h = b-a;
 break;
 case Y_AXIS:
 a = minPoint.y;
 b = maxPoint.y;
 h = b-a;
 break;
 case Z_AXIS:
 a = minPoint.z;
 b = maxPoint.z;
 h = b-a;
 break;
 }

 // Iterate over each vertex and compute its texture coordinate.
 for(UINT i = 0; i < temp->GetNumVertices(); ++i)
 {
 // Get the coordinates along the axes orthogonal to the
 // axis with which the cylinder is aligned.

 float x = 0.0f;
 float y = 0.0f;
 float z = 0.0f;
 switch( axis )
 {
 case X_AXIS:
 x = vertices[i].pos.y;
 z = vertices[i].pos.z;
 y = vertices[i].pos.x;
 break;
 case Y_AXIS:
 x = vertices[i].pos.x;
 z = vertices[i].pos.z;
 y = vertices[i].pos.y;
 break;
 case Z_AXIS:
 x = vertices[i].pos.x;
 z = vertices[i].pos.y;
 y = vertices[i].pos.z;
 break;
 }

 // Convert to cylindrical coordinates.

 float theta = atan2f(z, x);
 float y2    = y - b; // Transform [a, b]-->[-h, 0]

 // Transform theta from [0, 2*pi] to [0, 1] range and
 // transform y2 from [-h, 0] to [0, 1].

 float u = theta / (2.0f*D3DX_PI);
 float v = y2 / -h;

 // Save texture coordinates.
 vertices[i].tex0.x = u;
 vertices[i].tex0.y = v;
 }

 HR(temp->UnlockVertexBuffer());

 // Clone back to a hardware mesh.
 HR(temp->CloneMesh(D3DXMESH_MANAGED | D3DXMESH_WRITEONLY,
 elements, gd3dDevice, &mCylinder));

 ReleaseCOM(temp); 
 */

void Cylinder::Update(float dt)
{
	// We aren't changing anything, so just use the base update function
	BaseObject3D::Update(dt);
}

// A large chunk of this function was taken and adapted from Frank Luna's DirectX 11 Shapes Demo
// Credit goes to him for most of this.
/*
void Cylinder::calculateVertexBuffer(std::vector<Vertex>& vertices)
{
	float theta = 2.0f * D3DX_PI / m_SideFacetsNum;

	//cache base
	m_BaseIndex1 = (UINT)vertices.size();
	
	//Build top and bottom caps
	float y = 0.5f * m_Height;

 	// Top Cap
	for (UINT i = 0; i < m_SideFacetsNum; i++)
	{
		float x = m_Radius1 * cosf(i * theta);
		float z = m_Radius1 * sinf(i * theta);

		float u = x / m_Height + 0.5f;
		float v = z / m_Height + 0.5f;

		vertices.push_back(Vertex(x, y, z, 0.0f, 1.0f, 0.0f, u, v));
	}

	// cap center top vertex
	vertices.push_back(Vertex(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f));

	//cache center
	m_CenterIndex1 = (UINT)vertices.size() - 1;

	//cache second base
	m_BaseIndex2 = (UINT)vertices.size();

	// invert y
	y *= -1.0f;

	// Bottom Cap
	for (UINT i = 0; i < m_SideFacetsNum; i++)
	{
		float x = m_Radius2 * cosf(i * theta);
		float z = m_Radius2 * sinf(i * theta);

		float u = x / m_Height + 0.5f;
		float v = z / m_Height + 0.5f;

		vertices.push_back(Vertex(x, y, z, 0.0f, -1.0f, 0.0f, u, v));
	}

	//cap center bottom vertex
	vertices.push_back(Vertex(0.0f, y, 0.0f, 0.0f, -1.0f, 0.0f, 0.5f, 0.5f));

	// cache second center
	m_CenterIndex2 = (UINT)vertices.size() - 1;
}

// A large chunk of this function was taken and adapted from Frank Luna's DirectX 11 Shapes Demo
// Credit goes to him for most of this.
void Cylinder::calculateIndexBuffer(std::vector<WORD>& indices)
{
	//create indicies for caps

	// Top Cap
	for (UINT i = 0; i < m_SideFacetsNum; i++)
	{
		indices.push_back(m_CenterIndex1);
		indices.push_back(m_BaseIndex1 + i + 1);
		indices.push_back(m_BaseIndex1 + i);

		if (i == m_SideFacetsNum - 1)
		{
			// Draw the last line
			indices.push_back(m_CenterIndex1);
			indices.push_back(m_BaseIndex1);
			indices.push_back(m_CenterIndex1 - 1);
		}
	}

	// Bottom Cap
	for (UINT i = 0; i < m_SideFacetsNum; i++)
	{
		indices.push_back(m_CenterIndex2);
		indices.push_back(m_BaseIndex2 + i);
		indices.push_back(m_BaseIndex2 + i + 1);

		if (i == m_SideFacetsNum - 1)
		{
			// Draw the last line
			indices.push_back(m_CenterIndex2);
			indices.push_back(m_CenterIndex2 - 1);
			indices.push_back(m_BaseIndex2);
		}
	}

	// Calculate indices for center
	for (UINT i = 0; i < m_SideFacetsNum; i++)
	{
		if (i == m_SideFacetsNum - 1)
		{
			// Draw the last tris
			indices.push_back(m_BaseIndex1 + i);
			indices.push_back(m_BaseIndex1);
			indices.push_back(m_BaseIndex2 + i);

			indices.push_back(m_BaseIndex1);
			indices.push_back(m_BaseIndex2);
			indices.push_back(m_BaseIndex2 + i);

			break;
		}

		indices.push_back(m_BaseIndex1 + i);
		indices.push_back(m_BaseIndex1 + i + 1);
		indices.push_back(m_BaseIndex2 + i);

		indices.push_back(m_BaseIndex1 + i + 1);
		indices.push_back(m_BaseIndex2 + i + 1);
		indices.push_back(m_BaseIndex2 + i);
	}
}
*/