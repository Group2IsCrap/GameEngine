
cbuffer TextCB : register(b1)
{
    float2 cb_positionOffset;
    float2 cb_scale;
    
    float cb_antiAliasDistanceRange; // ? -> ? | Calculated by text code
    float cb_thickness; // 0 -> 1 | Set via text code
    int   cb_hasOutline;
    float cb_outlineWidth;

    float4 cb_colour;
    
    float4 cb_outlineColour;
}
