struct VSInput
{
	float3 Position : POSITION;
	float4 Color : COLOR;
	int EntityId : ENTITY_ID;
};

struct VSOutput
{
	float4 PositionCS : SV_POSITION;
	float4 Position : POSITION;
	float4 Color : COLOR;
	int EntityId : ENTITY_ID;
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
	output.EntityId = input.EntityId;
	return output;
}

struct PSOutput
{
	float4 Color : SV_TARGET0;
	int EntityId : SV_TARGET1;
};

PSOutput PSMain(VSOutput input)
{
	PSOutput output;
	output.Color = input.Color; // float4(1.0f, 0.0f, 0.0f, 1.0f);
	output.EntityId = input.EntityId;
	return output;
}