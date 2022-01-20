#ifndef __VS_INPUTS_HLSL__
#define __VS_INPUTS_HLSL__

struct UnlitVSInput
{
    float3 pos : POSITION;
    float2 texCoord : TEXCOORD;
};

struct SimpleLitVSInput
{
    float3 pos : POSITION;
    float2 texCoord : TEXCOORD;
};

struct FancyLitVSInput
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    float2 texCoord : TEXCOORD;
};

#endif //__VS_INPUTS_HLSL__
