/*
*	Vincent Loignon and Tyler Cifelli
*	Assignment 3
*	TextureMap.fx
*	I certify that this assignment is entirely our own work.
*
* Side Note: We worked on the algorithm for calculating the lighting together basing it
*			 off of Frank Luna's lighting chapter and other outside influence (cited in the
			 comments). The implementation afterwards was all based off the same algorithm.
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

uniform extern float gEnvMapStr;

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
	float2 TexCoord : TEXCOORD0;
};

struct OutputVS_PHONG
{
	float4 Position    : POSITION0;
	float3 Normal	   : TEXCOORD0;
	float3 WorldPos    : TEXCOORD1;
	float2 TexCoord    : TEXCOORD2;
};

// Phong shading done by Tyler Cifelli
// With the aid from frank luna's examples
OutputVS_PHONG TexturePhongVS(InputVS input)
{
	OutputVS_PHONG outVS = (OutputVS_PHONG)0;

	// Transform our normal into world space
	outVS.Normal = mul(float4(input.Normal, 0.0f), gWorldInvTrans).xyz;
	outVS.Normal = normalize(outVS.Normal);

	// Transform our vertex into world space
	outVS.WorldPos = mul(float4(input.Position, 1.0f), gWorld).xyz;

	// Transform our vertex into homogenous space
	outVS.Position = mul(float4(input.Position, 1.0f), gWVP);
	outVS.TexCoord = input.TexCoord;

	return outVS;
}

float4 TexturePhongPS(OutputVS_PHONG input) : COLOR
{
	// Renormalize our normal just to be safe
	float3 normal = normalize(input.Normal);

	float3 lightVec = normalize(gLightVec);

	// Calculate our eye (view) vector and normalize it
	float3 eyeVec = normalize(gEyePos - input.WorldPos);

	// Calculate the reflection vector from the light vector
	float3 reflectVec = reflect(-lightVec, normal);

	float3 specular = 0.0f;
  float3 diffuse = gColors.diffuse.rgb;
  float3 ambient = gColors.ambient.rgb;
  float alpha = gColors.diffuse.a;

	// Calculate our specular coloring
  float t = pow(max(dot(reflectVec, eyeVec), 0.0f), gColors.shininess);
  float s = max(dot(lightVec, normal), 0.0f);

	if (gUseTexture)
	{
		float4 texColor = tex2D(TextureSampler, input.TexCoord);

    diffuse *= texColor;
    ambient *= texColor;
    alpha *= texColor.a;
	}

  if (gUseEnvMap)
  {
    float3 envMapTex = reflect(-eyeVec, normal);
    float3 envMapColor = texCUBE(EnvMapSampler, envMapTex);

    diffuse *= (1.0f - gEnvMapStr);
    ambient *= (1.0f - gEnvMapStr);

    diffuse += gEnvMapStr * envMapColor;
    ambient += gEnvMapStr * envMapColor;
  }

  diffuse *= gDiffuseLight.rgb;
  diffuse *= s;
  ambient *= gAmbientLight.rgb;
  specular = t * (gColors.specular * gSpecularLight).rgb;

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
