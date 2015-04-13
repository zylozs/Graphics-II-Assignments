uniform extern float4x4 gWVP;
uniform extern texture  gEnvMap;

sampler EnvMapSampler = sampler_state
{
    Texture   = <gEnvMap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU  = WRAP;
    AddressV  = WRAP;
};

struct OUTPUT_VS
{
    float4 posW : POSITION0;
    float3 texCoord : TEXCOORD0;
};

struct InputVS
{
    float3 Position : POSITION0;
    float3 Normal : NORMAL0;
    float3 Tangent : TANGENT0;
    float3 BiNormal : BINORMAL0;
    float2 TexCoord : TEXCOORD0;
};

OUTPUT_VS CubeMapVS(float3 posL : POSITION0)
{
    OUTPUT_VS outVS = (OUTPUT_VS)0;
	// Set z = w so that z/w = 1 (i.e., skydome always on far plane).
    outVS.posW = mul(float4(posL, 1.0f), gWVP).xyww;

    // Use skymesh vertex position, in local space, as index into cubemap.
    outVS.texCoord = posL;

	return outVS;
}

float4 CubeMapPS(OUTPUT_VS input) : COLOR
{
    return texCUBE(EnvMapSampler, input.texCoord);
}

technique CubeMapTech
{
    pass P0
    {
        vertexShader = compile vs_2_0 CubeMapVS();
        pixelShader  = compile ps_2_0 CubeMapPS();
        FillMode = Solid;
		CullMode = None;
		ZFunc = Always; // Always write sky to depth buffer
		StencilEnable = true;
		StencilFunc   = Always;
		StencilPass   = Replace;
		StencilRef    = 0; // clear to zero
    }
}
