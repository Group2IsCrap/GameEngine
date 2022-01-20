
#include "../Shared/VSInputs.hlsli"
#include "../Shared/VSPSTransfers.hlsli"

UnlitVSPSTransfer main(UnlitVSInput input)
{
    UnlitVSPSTransfer output;
    output.pos = float4(input.pos, 1.0f);
    output.texCoord = float2(input.pos.x * 0.5f + 0.5f, -input.pos.y * 0.5f + 0.5f);
	return output;
}
