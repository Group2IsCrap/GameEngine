
#include "../Shared/VSPSTransfers.hlsli"

SamplerState samplerState : SAMPLER : register(s0);

Texture2D texture : TEXTURE : register(t0);

float4 main(UnlitLitVSPSTransfer input) : SV_TARGET
{
    return texture.Sample(samplerState, input.texCoord);
}
