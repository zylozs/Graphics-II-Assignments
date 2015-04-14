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

uniform extern float3 gAttenuation;

uniform extern MaterialColor gColors;

float4 gAmbientLight = float4(0.4f, 0.4f, 0.4f, 0.4f);
float4 gDiffuseLight = float4(1.0f, 1.0f, 1.0f, 1.0f);
float4 gSpecularLight = float4(1.0f, 1.0f, 1.0f, 1.0f);

float gAmbientBlend = float(0.2f);
float gDiffuseBlend = float(0.65f);
float gSpecularBlend = float(0.5f);

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
    float3 WorldPos    : TEXCOORD0;
    float2 TexCoord    : TEXCOORD1;
	float3 EyeT		   : TEXCOORD2;
	float3 LightDir    : TEXCOORD3;
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

    // Sample our texture's color
    float4 texColor = tex2D(TextureSampler, input.TexCoord);

    // Sample the normal map, Expand from [0, 1] compressed interval
    // to true [-1, 1] interval, Make it a unit vector.
    float3 normalT = tex2D(NormalMapSampler, input.TexCoord);
    normalT = 2.0f * normalT - 1.0f;
    normalT = normalize(normalT);

    // Calculate the reflection vector from the light vector
    float3 reflectVec = reflect(-lightVec, normalT);

    // Calculate coefficients
    float t = pow(max(dot(reflectVec, eyeT), 0.0f), gColors.shininess);
    float s = max(dot(lightVec, normalT), 0.0f);

    // Declare our values and initialze them (with blending)
	float3 specular = t * (gColors.specular * gSpecularLight).rgb * gSpecularBlend;
    float3 diffuse = s * (gColors.diffuse * gDiffuseLight).rgb * gDiffuseBlend;
    float3 ambient = (gColors.ambient * gAmbientLight).rgb * gAmbientBlend;
    float alpha = texColor.a * gColors.diffuse.a;

    // Return the color
    return float4((ambient + diffuse) * texColor.rgb + specular, alpha);
}

OutputVS_PHONG PhongPointLightVS(InputVS input)
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

    // Transform our vertex into world space
    outVS.WorldPos = mul(float4(input.Position, 1.0f), gWorld).xyz;

    // Calculate light direction, then transpose
    float3 lightVec = normalize(gLightVec - outVS.WorldPos);
    float3 lightDir = mul(float4(lightVec, 0.0f), gWorldInvTrans).xyz;
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

float4 PhongPointLightPS(OutputVS_PHONG input) : COLOR
{
	// Renormalize our values just to be safe
	float3 lightVec = normalize(input.LightDir);
    float3 eyeT = normalize(input.EyeT);

    // Sample our texture's color
    float4 texColor = tex2D(TextureSampler, input.TexCoord);

    // Sample the normal map, Expand from [0, 1] compressed interval
    // to true [-1, 1] interval, Make it a unit vector.
    float3 normalT = tex2D(NormalMapSampler, input.TexCoord);
    normalT = 2.0f * normalT - 1.0f;
    normalT = normalize(normalT);

    // Calculate the reflection vector from the light vector
    float3 reflectVec = reflect(-lightVec, normalT);

    // Calculate coefficients
    float t = pow(max(dot(reflectVec, eyeT), 0.0f), gColors.shininess);
    float s = max(dot(lightVec, normalT), 0.0f);

    // Attenuation
    float d = length(gLightVec - input.WorldPos);
    float A = gAttenuation.x + gAttenuation.y * d + gAttenuation.z * d * d;
    float atten = 1.0f / dot(gAttenuation, float3(1.0f, d, d*d));

    // Declare our values and initialze them (with blending)
	float3 specular = t * (gColors.specular * gSpecularLight).rgb * gSpecularBlend;
    float3 diffuse = s * (gColors.diffuse * gDiffuseLight).rgb * gDiffuseBlend;
    float3 ambient = (gColors.ambient * gAmbientLight).rgb * gAmbientBlend;
    float alpha = texColor.a * gColors.diffuse.a;

    // Return the color
    return float4((ambient + (diffuse / A)) * texColor.rgb + (specular / A), alpha);
}

technique PhongTech
{
	pass P0
	{
		VertexShader = compile vs_2_0 TexturePhongVS();
		PixelShader = compile ps_2_0 TexturePhongPS();
	}
};

technique PhongPointTech
{
    pass P0
    {
        VertexShader = compile vs_2_0 PhongPointLightVS();
        PixelShader = compile ps_2_0 PhongPointLightPS();
    }
};
