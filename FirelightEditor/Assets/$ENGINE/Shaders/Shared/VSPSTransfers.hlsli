#ifndef __VSPS_TRANSFERS_HLSL__
#define __VSPS_TRANSFERS_HLSL__

struct UnlitVSPSTransfer
{
    float4 pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

struct UnlitColourVSPSTransfer
{
    float4 pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
    float4 colour : COLOUR;
};

struct SimpleLitVSPSTransfer
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
    float3 worldPos : WORLD_POS;
};

struct FancyLitVSPSTransfer
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    float2 texCoord : TEXCOORD;
    float3 worldPos : WORLD_POS;
};

#endif //__VSPS_TRANSFERS_HLSL__
