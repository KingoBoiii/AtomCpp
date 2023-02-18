struct VSInput
{
	float3 Position : POSITION;
	float4 Color : COLOR;
};

struct VSOutput
{
	float4 PositionCS : SV_POSITION;
	float4 Position : POSITION;
	float4 Color : COLOR;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
	float4x4 ProjectionViewMatrix;
}

VSOutput VSMain(VSInput input)
{
	float4 pos = float4(input.Position, 1.0);

	VSOutput output = (VSOutput)0; // zero the memory first
	output.PositionCS = mul(ProjectionViewMatrix, pos);
	output.Position = pos;
	output.Color = input.Color;
	return output;
}

struct PSOutput
{
	float4 Color : SV_TARGET;
};

PSOutput PSMain(VSOutput input)
{
	PSOutput output;
	output.Color = input.Color; // float4(1.0f, 0.0f, 0.0f, 1.0f);
	return output;
}