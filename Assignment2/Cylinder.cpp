#include "Cylinder.h"
#include "3DClasses\Vertex.h"

Cylinder::Cylinder(float radius1, float radius2, float height, uint slices, uint stacks)
	: m_Radius1(radius1), m_Radius2(radius2),
	m_Height(height), m_SliceCount(slices),
	m_StackCount(stacks)
{
	m_StackHeight = m_Height / m_StackCount;

	//calculate radius step per stack level
	m_RadiusStep = (m_Radius1 - m_Radius2) / m_StackCount;
	
}

Cylinder::~Cylinder()
{

}

void Cylinder::Create(IDirect3DDevice9* gd3dDevice)
{
	// We aren't changing anything, so just use the base create function
	BaseObject3D::Create(gd3dDevice);
}

void Cylinder::Render(IDirect3DDevice9* gd3dDevice, D3DXMATRIX& view, D3DXMATRIX& projection)
{
	// We aren't changing anything, so just use the base render function
	BaseObject3D::Render(gd3dDevice, view, projection);
}

void Cylinder::Update(float dt)
{
	// We aren't changing anything, so just use the base update function
	BaseObject3D::Update(dt);
}

// A large chunk of this function was taken and adapted from Frank Luna's DirectX 11 Shapes Demo
// Credit goes to him for most of this.
void Cylinder::calculateVertexBuffer(std::vector<VertexPos>& vertices)
{
	float theta = 2.0f * PI / m_SliceCount;

	for  (uint i = 0; i < m_StackCount + 1; i++)
	{

		//calculate radius and height values
		float y = -0.5f * m_Height + i * m_StackHeight;
		float r = m_Radius2 + i * m_RadiusStep;

		//create vertices of ring
		for (uint j = 0; j < m_SliceCount; j++)
		{
			VertexPos vertex;

			//Sin and Cos values
			float c = cosf(j * theta);
			float s = sinf(j * theta);

			//place vertex position
			vertex.pos.x = r * c;
			vertex.pos.y = y;
			vertex.pos.z = r * s;

			//?
			//vertex.TexC.x = (float)j / sliceCount;
			//vertex.TexC.y = 1.0f - (float)i / stackCount;

			//vertex.TangentU = XMFLOAT3(-s, 0.0f, c);

			float dr = m_Radius2 - m_Radius1;

			//?
			//XMFLOAT3 bitangent(dr*c, -height, dr*s);

			//XMVECTOR T = XMLoadFloat3(&vertex.TangentU);
			//XMVECTOR B = XMLoadFloat3(&bitangent);
			//XMVECTOR N = XMVector3Normalize(XMVector3Cross(T, B));
			//XMStoreFloat3(&vertex.Normal, N);

			vertices.push_back(vertex);
		}
	}
	
	//Build top and bottom caps
	float y = -0.5f * m_Height;

	// ring vertices
	//duplicated due to normals differ and texture coordinates
	for (uint i = 0; i < m_SliceCount; i++)
	{
		//top cap
		float xt = m_Radius1 * cosf(i * theta);
		float zt = m_Radius1 * sinf(i * theta);

		//scalars
		float ut = xt / m_Height + 0.5f;
		float vt = zt / m_Height + 0.5f;

		//DX11 line
		//meshData.Vertices.push_back( Vertex(x, y, z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v) );
		vertices.push_back(VertexPos(xt, y, zt));

		//bottom cap
		float xb = m_Radius2 / m_Height + 0.5f;
		float zb = m_Radius2 / m_Height + 0.5f;

		//scalars
		float ub = xb / m_Height + 0.5f;
		float vb = zb / m_Height + 0.5f;

		//DX11 line
		//meshData.Vertices.push_back( Vertex(x, y, z, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v) );
		vertices.push_back(VertexPos(xb, y, zb));
	}

	// cap center top vertex
	// meshData.Vertices.push_back( Vertex(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f) );
	vertices.push_back(VertexPos(0.0f, y, 0.0f));

	//cap center bottom vertex
	//meshData.Vertices.push_back( Vertex(0.0f, y, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f) );
	vertices.push_back(VertexPos(0.0f, y, 0.0f));

	//cache base
	m_BaseIndex = (uint)vertices.size();

	//cache center
	m_CenterIndex = (uint)vertices.size() - 1;
}

// A large chunk of this function was taken and adapted from Frank Luna's DirectX 11 Shapes Demo
// Credit goes to him for most of this.
void Cylinder::calculateIndexBuffer(std::vector<WORD>& indices)
{
	//calculate vertex count for ring, it has one more  because of different coordinates
	uint ringVertexCount = m_StackCount + 1;

	//generate indicies
	for (uint i = 0; i < m_StackCount; i++)
	{
		for (uint j = 0; j < m_SliceCount; j++)
		{
			indices.push_back(i * ringVertexCount + j);
			indices.push_back((i + 1) * ringVertexCount + j);
			indices.push_back((i + 1) * ringVertexCount + (j + 1));

			indices.push_back(i * ringVertexCount + j);
			indices.push_back((i + 1) * ringVertexCount + (j + 1));
			indices.push_back(i * ringVertexCount + (j + 1));
		}
	}

	//create indicies for caps
	for (uint i = 0; i < m_SliceCount; i++)
	{
		//top
		indices.push_back(m_CenterIndex);
		indices.push_back(m_BaseIndex + (i + 1));
		indices.push_back(m_BaseIndex + i);

		//bottom
		indices.push_back(m_CenterIndex);
		indices.push_back(m_BaseIndex + i);
		indices.push_back(m_BaseIndex + (i + 1));
	}

}