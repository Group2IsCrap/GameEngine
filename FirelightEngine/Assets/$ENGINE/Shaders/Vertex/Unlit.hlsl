
#include "../Shared/VSInputs.hlsli"
#include "../Shared/VSPSTransfers.hlsli"

UnlitLitVSPSTransfer main(UnlitVSInput input)
{
    UnlitLitVSPSTransfer output;
    output.pos = float4(input.pos, 1.0f);
    output.texCoord = input.texCoord;
	return output;
}
