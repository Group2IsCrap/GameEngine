
#include "../Shared/VSInputs.hlsli"
#include "../Shared/VSPSTransfers.hlsli"
#include "../Shared/TextData.hlsli"

UnlitVSPSTransfer main(UnlitVSInput input)
{
    UnlitVSPSTransfer output;
    output.pos = float4(input.pos.xy * cb_scale + cb_positionOffset, input.pos.z, 1.0f);
    output.texCoord = input.texCoord;
    return output;
}
