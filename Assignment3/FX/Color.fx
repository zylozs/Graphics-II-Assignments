uniform extern float4x4 gWorld;
uniform extern float4x4 gWorldInvTrans;
uniform extern float4x4 gWVP;

uniform extern float3 gEyePosW;
uniform extern float3 gLightPosition;
uniform extern float4 gColor;

uniform extern float4 gDiffuseColor;
uniform extern float4 gSpecularColor;
uniform extern float gSpecularPower;

uniform extern bool gUseDiffuse;
uniform extern bool gUseSpecular;

float4 gAmbientLight = float4(0.4f, 0.4f, 0.4f, 0.4f);
float4 gDiffuseLight = float4(1.0f, 1.0f, 1.0f, 1.0f);
float4 gSpecularLight = float4(1.0f, 1.0f, 1.0f, 1.0f);
float3 gAttenuation = float3(1.0f, 0.0f, 0.0f);

struct InputVS
{
	float3 Position : POSITION0;
	float3 Normal : NORMAL0;
	float2 TexCoord : TEXCOORD0;
};

struct OutputVS
{
	float4 Position : POSITION0;
	float4 Color : COLOR1;
};

OutputVS ColorPhongVS(InputVS input)
{
	OutputVS outVS = (OutputVS)0;

	return outVS;
}

float4 ColorPhongPS(OutputVS input) : COLOR
{
	return float4(0.0f, 0.0f, 0.0f, 1.0f);
}

OutputVS ColorGouraudVS(InputVS input)
{
	OutputVS outVS = (OutputVS)0;

	// Transform our normals to world space and normalize them for use
	float3 normal = mul(float4(input.Normal, 0.0f), gWorldInvTrans).xyz;
		normal = normalize(normal);

	// Transform our position into world space
	float3 pos = mul(float4(input.Position, 1.0f), gWorld).xyz;

	// Calculate our light vector and normalize it
	float3 lightVec = normalize(gLightPosition - pos);

	// Calculate our eye (view) vector and normalize it
	float3 eyeVec = normalize(gEyePosW - pos);

	// Calculate the reflection vector from the light vector
	float3 reflectVec = reflect(-lightVec, normal);

	// Calculate our ambient coloring
	float3 ambient = (gColor * gAmbientLight).rgb;

	float diffuse = 0.0f;
	float specular = 0.0f;

	if (gUseDiffuse)
	{
		// Calculate our diffuse coloring
		float s = max(dot(input.Normal, lightVec), 0.0f);
		diffuse = s * (gDiffuseColor * gDiffuseLight).rgb;
	}

	if (gUseSpecular)
	{
		// Calculate our specular coloring
		float t = pow(max(dot(reflectVec, eyeVec), 0.0f), gSpecularPower);
		specular = t * (gSpecularColor * gSpecularLight).rgb;
	}

	// Calculate the attenuation of our lighting
	float dist = distance(gLightPosition, pos);
	float atten = gAttenuation.x + gAttenuation.y * dist + gAttenuation.z * dist * dist;

	// Combine all the colors together to get our final color
	float3 color = ambient + ((diffuse + specular) / 2);

	// Assign the values of our colors to the output and set our alpha to the diffuse color's alpha
	outVS.Color = float4(color, gDiffuseColor.a);

	// Calculate our position in homogeneous space
	outVS.Position = mul(float4(input.Position, 1.0f), gWVP);

	return outVS;
}

float4 ColorGouraudPS(OutputVS input) : COLOR
{
	return input.Color;
}

// Vertex shader
OutputVS ColorVS(InputVS input)
{
	// Zero out our output.
	OutputVS outVS = (OutputVS)0;

	// Transform to homogeneous clip space.
	outVS.Position = mul(float4(input.Position, 1.0f), gWVP);

	outVS.Color = gColor;

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

technique ColorPhongTech
{
	pass P0
	{
		VertexShader = compile vs_2_0 ColorPhongVS();
		PixelShader = compile ps_2_0 ColorPhongPS();
	}
};

technique ColorGouraudTech
{
	pass P0
	{
		VertexShader = compile vs_2_0 ColorGouraudVS();
		PixelShader = compile ps_2_0 ColorGouraudPS();
	}
};