
#include "../Shared/VSPSTransfers.hlsli"
#include "../Shared/TextData.hlsli"

SamplerState samplerState : SAMPLER : register(s0);

Texture2D textGlyphs : TEXTURE : register(t0);

float getLinearProgress(float edge0, float edge1, float t)
{
    return clamp((t - edge0) / (edge1 - edge0), 0.0f, 1.0f);
}

float4 main(UnlitVSPSTransfer input) : SV_TARGET
{
    float distance = textGlyphs.Sample(samplerState, input.texCoord).a;
    
    float thicknessOffset = (cb_thickness * 2.0f - 1.0f) * -0.05f;

    float solidEdge0 = 0.5f - cb_antiAliasDistanceRange * 0.5f + thicknessOffset;
    float solidEdge1 = 0.5f + cb_antiAliasDistanceRange * 0.5f + thicknessOffset;

    float solidity = 0.0f;
    float4 finalColour = cb_colour;
    
    if (cb_hasOutline)
    {
        float outlineEdge0 = solidEdge0 - cb_outlineWidth * 0.35f;
        float outlineEdge1 = solidEdge1 - cb_outlineWidth * 0.35f;
        
        solidity = getLinearProgress(outlineEdge0, outlineEdge1, distance);
        
        float isPixelOutline = getLinearProgress(solidEdge1, solidEdge0, distance);
        
        finalColour = lerp(cb_colour, cb_outlineColour, isPixelOutline);
    }
    else
    {
        solidity = getLinearProgress(solidEdge0, solidEdge1, distance);
    }
    
    return float4(finalColour.rgb, solidity * finalColour.a);
}
