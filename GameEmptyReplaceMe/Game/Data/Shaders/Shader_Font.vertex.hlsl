cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
    matrix mvp;
    float4 tint;
};

struct VertexShaderInput
{
    float2 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VertexShaderOutput
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;

    // Transform the vertex position into projected space.
    float4 pos = float4(input.pos, 0.0f, 1.0f);
    pos = mul(pos, mvp);
    output.pos = pos;
    
    // Pass through the tint and uv without modification.
    output.uv = input.uv;
    output.color = tint;

    return output;
}
