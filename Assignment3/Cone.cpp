#include "Cone.h"
#include "3DClasses\Vertex.h"

Cone::Cone(float radius, float height, UINT sideFacetsNum)
	: Cylinder(radius, (UINT)height, sideFacetsNum)
{
	//Second radius is zero because cone
	m_Radius2 = 0;
}

Cone::~Cone()
{

}

void Cone::Create(IDirect3DDevice9* gd3dDevice)
{
	D3DXCreateCylinder(gd3dDevice, m_Radius1, 0, m_Height, m_SideFacetsNum, m_SideFacetsNum, &m_Mesh, mp_Buffer);

	generateUVs();
}

void Cone::Update(float dt)
{
	// We aren't changing anything, so just use the base update function
	BaseObject3D::Update(dt);
}

void Cone::generateUVs()
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
void Cone::calculateVertexBuffer(std::vector<Vertex>& vertices)
{
	float theta = 2.0f * D3DX_PI / m_SideFacetsNum;

	//build bottom cap with top center index
	float y = 0.5f * m_Height;

	//Push top vertex
	vertices.push_back(Vertex(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f));

	//cache center
	m_CenterIndex1 = (UINT)vertices.size() - 1;

	//cache base
	m_BaseIndex2 = (UINT)vertices.size();

	//invert y
	y *= -1.0f;

	//Bottom cap
	for (UINT i = 0; i < m_SideFacetsNum; i++)
	{
		float x = m_Radius1 * cosf(i * theta);
		float z = m_Radius1 * sinf(i * theta);

		float u = x / m_Height + 0.5f;
		float v = z / m_Height + 0.5f;

		vertices.push_back(Vertex(x, y, z, 0.0f, -1.0f, 0.0f, u, v));
	}

	//cap center for bottom vertex
	vertices.push_back(Vertex(0.0f, y, 0.0f, 0.0f, -1.0f, 0.0f, 0.5f, 0.5f));

	//cache second center
	m_CenterIndex2 = (UINT)vertices.size() - 1;

}

void Cone::calculateIndexBuffer(std::vector<WORD>& indices)
{
	// Pointy part
	for (UINT i = 0; i < m_SideFacetsNum - 1; i++)
	{
		indices.push_back(m_CenterIndex1);
		indices.push_back(m_BaseIndex2 + i + 1);
		indices.push_back(m_BaseIndex2 + i);

		if (i == m_SideFacetsNum - 2)
		{
			indices.push_back(m_CenterIndex1);
			indices.push_back(m_BaseIndex2);
			indices.push_back(m_CenterIndex2 - 1);
		}
	}

	//Bottom Cap
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
}
*/