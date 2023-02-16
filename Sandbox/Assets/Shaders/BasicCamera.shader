struct VSInput
{
	float3 Position : POSITION;
};

struct VSOutput
{
	float4 Position : SV_POSITION;
};

VSOutput VSMain(VSInput input)
{
	VSOutput output = (VSOutput)0; // zero the memory first
	output.Position = float4(input.Position, 1.0);
	return output;
}

struct PSOutput
{
	float4 Color : SV_TARGET;
};

PSOutput PSMain(VSOutput input)
{
	PSOutput output;
	output.Color = float4(1.0f, 0.0f, 0.0f, 1.0f);
	return output;
}