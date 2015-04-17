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

uniform extern texture gEnvMap;
sampler EnvMapSampler = sampler_state
{
    Texture = <gEnvMap>;
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

uniform extern bool gUseTexture;
uniform extern bool gUseEnvMap;
uniform extern bool gUseNormalMap;

uniform extern float gEnvMapStr;
uniform extern float gNormalMapStr;

float4 gAmbientLight = float4(0.4f, 0.4f, 0.4f, 0.4f);
float4 gDiffuseLight = float4(1.0f, 1.0f, 1.0f, 1.0f);
float4 gSpecularLight = float4(1.0f, 1.0f, 1.0f, 1.0f);

float gAmbientBlend = float(0.2f);
float gDiffuseBlend = float(0.65f);
float gSpecularBlend = float(0.15f);

struct InputVS
{
    float3 Position : POSITION0;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;
    float3 BiNormal : BINORMAL0;
    float2 TexCoord : TEXCOORD0;
};

struct OutputVS_PHONG
{
    float4 Position    : POSITION0;
    float3 Normal	   : TEXCOORD0;
    float3 WorldPos    : TEXCOORD1;
    float2 TexCoord    : TEXCOORD2;
	float3 EyeT		   : TEXCOORD3;
	float3 LightDir    : TEXCOORD4;
};

// Phong shading done by Tyler Cifelli & Vincent Loignon
// With the aid from frank luna's examples
OutputVS_PHONG TexturePhongVS(InputVS input)
{
    OutputVS_PHONG outVS = (OutputVS_PHONG)0;

	//Generate TBN-Matrix
    //TBN calculations by Tyler Cifelli
	float3x3 TBN;
    TBN[0] = input.Tangent;
    TBN[1] = input.BiNormal;
    TBN[2] = input.Normal;

	//transform to tangent space
	float3x3 TBNinTangent = transpose(TBN);

    // Transform our normal into world space
    outVS.Normal = mul(float4(input.Normal, 0.0f), gWorldInvTrans).xyz;
    outVS.Normal = normalize(outVS.Normal);

    // Transform our vertex into world space
    outVS.WorldPos = mul(float4(input.Position, 1.0f), gWorld).xyz;

    // Calculate light direction, then transpose
    float3 lightDir = mul(float4(gLightVec, 0.0f), gWorldInvTrans).xyz;
    outVS.LightDir = mul(lightDir, TBNinTangent);

    // Calculate our eye (view) vector, then transpose
    float3 eyePosL = mul(float4(gEyePos, 1.0f), gWorldInvTrans);
	float3 eyeVec = eyePosL - input.Position;
	outVS.EyeT = mul(eyeVec, TBNinTangent);

    // Transform our vertex into homogenous space
    outVS.Position = mul(float4(input.Position, 1.0f), gWVP);
    outVS.TexCoord = input.TexCoord;

    return outVS;
}

float4 TexturePhongPS(OutputVS_PHONG input) : COLOR
{
    // Renormalize our values just to be safe
	float3 lightVec = normalize(input.LightDir);
    float3 eyeT = normalize(input.EyeT);
    float3 normalW = normalize(input.Normal);

    // Sample our texture's color
    float4 texColor = tex2D(TextureSampler, input.TexCoord);

    // Normal Map code by Tyler Cifelli
    // Sample the normal map, Expand from [0, 1] compressed interval
    // to true [-1, 1] interval, Make it a unit vector.
    float3 normalT = tex2D(NormalMapSampler, input.TexCoord);
    normalT = 2.0f * normalT - 1.0f;
    normalT = normalize(normalT);

    // Calculate the reflection vector from the light vector
    float3 reflectVec;

    // Make sure to calculate reflection vector based off the proper
    // set of normals
    if (gUseNormalMap)
        reflectVec = reflect(-lightVec, normalT);
    else
        reflectVec = reflect(-lightVec, normalW);

    float3 envMapTex;

    // Make sure to calculate the texCoord for the environment
    // map based off the proper set of normals
    if (!gUseNormalMap || gNormalMapStr == 0.0f)
        envMapTex = reflect(-eyeT, normalW);
    else
        envMapTex = reflect(-eyeT, normalT);

    float3 envMapColor = texCUBE(EnvMapSampler, envMapTex);

    // Calculate coefficients
    float t = pow(max(dot(reflectVec, eyeT), 0.0f), gColors.shininess);
    float s = max(dot(lightVec, normalW), 0.0f);

    // Normal Map code by Tyler Cifelli
    if (gUseNormalMap)
    {
        s *= (1.0f - gNormalMapStr);
        s += gNormalMapStr * max(dot(lightVec, normalT), 0.0f);
    }

    // Declare our values and initialze them (with blending)
	float3 specular = t * (gColors.specular * gSpecularLight).rgb * gSpecularBlend;
    float3 diffuse = s * (gColors.diffuse * gDiffuseLight).rgb * gDiffuseBlend;
    float3 ambient = (gColors.ambient * gAmbientLight).rgb * gAmbientBlend;
    float alpha = gColors.diffuse.a;

    if (gUseTexture)
    {
        diffuse *= texColor.rgb;
        ambient *= texColor.rgb;
        alpha *= texColor.a;
    }

    // Environment Map code by Vincent Loignon
    if (gUseEnvMap)
    {
        specular *= (1.0f - gEnvMapStr);
        specular += gEnvMapStr * envMapColor;
    }

    // Return the color
    return float4(ambient + diffuse + specular, alpha);
}

technique PhongTech
{
	pass P0
	{
		VertexShader = compile vs_2_0 TexturePhongVS();
		PixelShader = compile ps_2_0 TexturePhongPS();
	}
};
