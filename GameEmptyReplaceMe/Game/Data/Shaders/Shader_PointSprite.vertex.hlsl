cbuffer ConstantBuffer //: register(b0)
{
    matrix mvp;
};

struct VertexShaderInput
{
	float2 pos : POSITION;
	float2 uv : TEXCOORD;
	float4 color : COLOR;
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

	output.pos = mul( float4(input.pos, 0.0f, 1.0f), mvp );
    
	// Pass through the color and uv without modification.
    output.uv = input.uv;
	output.color = input.color;

	return output;
}
