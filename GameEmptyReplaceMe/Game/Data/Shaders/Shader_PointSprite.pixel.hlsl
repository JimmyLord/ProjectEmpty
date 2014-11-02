struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

Texture2D shaderTexture;
SamplerState SampleType;

float4 main(PixelShaderInput input) : SV_TARGET
{
    float4 textureColor = shaderTexture.Sample( SampleType, input.uv );

    return textureColor * input.color;
}
