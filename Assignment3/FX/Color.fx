uniform extern float4x4 gWorldViewProjectMatrix;
uniform extern float4 gColor;

struct InputVS
{
	float3 Position : POSITION0;
	float3 Normal : NORMAL0;
	float2 TexCoord : TEXCOORD0;
};

struct OutputVS
{
	float4 Position : POSITION0;
	float4 Color : COLOR0;
};

// Vertex shader
OutputVS ColorVS(InputVS input)
{
	// Zero out our output.
	OutputVS outVS = (OutputVS)0;

	// Transform to homogeneous clip space.
	outVS.Position = mul(float4(input.Position, 1.0f), gWorldViewProjectMatrix);

	outVS.Color = gColor;
	//outVS.Color = abs(float4(input.Normal, 1));

	// Done--return the output.
	return outVS;
}

// Pixel shader
float4 ColorPS(OutputVS input) : COLOR
{
	return input.Color;
}

technique ColorTech
{
	pass P0
	{
		// Specify the vertex and pixel shader associated
		// with this pass.
		VertexShader = compile vs_2_0 ColorVS();
		PixelShader = compile ps_2_0 ColorPS();
	}
}