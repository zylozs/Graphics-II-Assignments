uniform extern float4x4 gWorld;
uniform extern float4x4 gWorldInvTrans;
uniform extern float4x4 gWVP;

uniform extern float3 gEyePosW;
uniform extern float3 gLightPosition;

uniform extern float4 gDiffuseColor;
uniform extern float4 gSpecularColor;
uniform extern float gSpecularPower;

float4 gAmbientLight = float4(0.4f, 0.4f, 0.4f, 0.4f);
float4 gDiffuseLight = float4(1.0f, 1.0f, 1.0f, 1.0f);
float4 gSpecularLight = float4(1.0f, 1.0f, 1.0f, 1.0f);
float3 gAttenuation = float3(1.0f, 0.0f, 0.0f);

uniform extern texture gTexture;
sampler TextureSampler = sampler_state
{
      Texture = <gTexture>;
      MinFilter = LINEAR;
      MagFilter = LINEAR;
};

struct InputVS
{
	float3 Position : POSITION0;
	float3 Normal : NORMAL0;
	float2 TexCoord : TEXCOORD0;
};

struct OutputVS
{
	float4 Position : POSITION0;
	float2 TexCoord : TEXCOORD0;
};

OutputVS TextureMapVS(InputVS input)
{
	OutputVS outVS = (OutputVS)0;

	outVS.Position = mul(float4(input.Position, 1.0f), gWVP);

	outVS.TexCoord = input.TexCoord;

	return outVS;
}

float4 TextureMapPS(OutputVS input) : COLOR
{
	// Get texel from texture map that gets mapped to this pixel.
	/*float3 texColor = tex2D(TextureSampler, input.TexCoord).rgb;

	// Combine texture color with color generated by lighting.
	float3 diffuse = c.rgb * texColor;

	// Add in the specular term separately.
	return float4(diffuse + spec.rgb, c.a);*/

	return tex2D(TextureSampler, input.TexCoord);
}

technique TextureMapTech
{
	pass P0
	{
		VertexShader = compile vs_2_0 TextureMapVS();
		PixelShader = compile ps_2_0 TextureMapPS();
	}
}