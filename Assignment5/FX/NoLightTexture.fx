/*
*	Vincent Loignon and Tyler Cifelli
*	Assignment 4
*	TextureMap.fx
*	I certify that this assignment is entirely our own work.
*
*/

uniform extern texture gTexture;
sampler TextureSampler = sampler_state
{
    Texture = <gTexture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

uniform extern texture gNormalMap;
sampler NormalMapSampler = sampler_state
{
    Texture = <gNormalMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

struct MaterialColor
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float shininess;
};

uniform extern float4x4 gWorld;
uniform extern float4x4 gWorldInvTrans;
uniform extern float4x4 gWVP;

uniform extern float3 gEyePos;
uniform extern float3 gLightVec;

uniform extern MaterialColor gColors;

struct InputVS
{
    float3 Position : POSITION0;
    float2 TexCoord : TEXCOORD0;
};

struct OutputVS
{
    float4 Position    : POSITION0;
    float2 TexCoord    : TEXCOORD1;
};

OutputVS NoLightTextureVS(InputVS input)
{
    OutputVS outVS = (OutputVS)0;

    // Transform our vertex into homogenous space
    outVS.Position = mul(float4(input.Position, 1.0f), gWVP);
    outVS.TexCoord = input.TexCoord;

    return outVS;
}

float4 NoLightTexturePS(OutputVS input) : COLOR
{
    float3 lightVec = (1.0f, 1.0f, 1.0f);

    // Sample our texture's color
    float4 texColor = tex2D(TextureSampler, input.TexCoord);

    // Sample the normal map, Expand from [0, 1] compressed interval
    // to true [-1, 1] interval, Make it a unit vector.
    float3 normalT = tex2D(NormalMapSampler, input.TexCoord);
    normalT = 2.0f * normalT - 1.0f;
    normalT = normalize(normalT);

    float s = max(dot(lightVec, normalT), 0.0f);
    float alpha = texColor.a * gColors.diffuse.a;

    return float4((gColors.diffuse * s) * texColor.rgb, alpha);
}

technique NoLightTextureTech
{
	pass P0
	{
		VertexShader = compile vs_2_0 NoLightTextureVS();
		PixelShader = compile ps_2_0 NoLightTexturePS();
	}
};
