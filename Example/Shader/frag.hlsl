cbuffer Uniforms : register(b0)
{
    float intensity;
}

struct PS_INPUT
{
    float4 pos : SV_POSITION;
	float2 texCoord : TEXCOORD;
    float4 col : COLOR0;
};

Texture2D colorMap: register(t0);
SamplerState colorMapSampler: register(s0);

float4 main(PS_INPUT vertexOut) : SV_Target
{
	float4 color = lerp(vertexOut.col * intensity, colorMap.Sample(colorMapSampler, vertexOut.texCoord), 0.5f);
	return color;
}