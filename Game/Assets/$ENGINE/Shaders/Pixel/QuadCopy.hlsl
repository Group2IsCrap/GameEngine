
#include "../Shared/VSPSTransfers.hlsli"

SamplerState samplerState : SAMPLER : register(s0);

Texture2D unlitTexture : TEXTURE : register(t0);

float4 main(UnlitVSPSTransfer input) : SV_TARGET
{
    return unlitTexture.Sample(samplerState, input.texCoord);
}
