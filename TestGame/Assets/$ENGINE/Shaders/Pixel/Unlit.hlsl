
#include "../Shared/VSPSTransfers.hlsli"

SamplerState samplerState : SAMPLER : register(s0);

Texture2D unlitTexture : TEXTURE : register(t0);

float4 main(UnlitLitVSPSTransfer input) : SV_TARGET
{
    float4 textureSample = unlitTexture.Sample(samplerState, input.texCoord);
    float3 mappedColour = pow(textureSample.rgb, 1.0f / 2.2f);
    return float4(mappedColour, textureSample.a);
}
