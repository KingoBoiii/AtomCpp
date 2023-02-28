struct VSInput
{
	float3 WorldPosition : WORLD_POSITION;
	float3 LocalPosition : LOCAL_POSITION;
	float4 Color : COLOR;
	float Thickness: THICKNESS;
	float Fade: FADE;
};

struct VSOutput
{
	float4 PositionCS : SV_POSITION;
	float4 WorldPosition : WORLD_POSITION;
	float3 LocalPosition : LOCAL_POSITION;
	float4 Color : COLOR;
	float Thickness : THICKNESS;
	float Fade : FADE;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
	float4x4 ProjectionViewMatrix;
}

VSOutput VSMain(VSInput input)
{
	float4 pos = float4(input.WorldPosition, 1.0);

	VSOutput output = (VSOutput)0; // zero the memory first
	output.PositionCS = mul(ProjectionViewMatrix, pos);
	output.WorldPosition = pos;
	output.LocalPosition = input.LocalPosition;
	output.Color = input.Color;
	output.Thickness = input.Thickness;
	output.Fade = input.Fade;
	return output;
}

struct PSOutput
{
	float4 Color : SV_TARGET;
};

PSOutput PSMain(VSOutput input)
{
	float distance = 1.0f - length(input.LocalPosition);
	float circleAlpha = smoothstep(0.0f, input.Fade, distance);
	circleAlpha *= smoothstep(input.Thickness + input.Fade, input.Thickness, distance);

	if(circleAlpha == 0.0f)
	{
		discard;
	}

	float4 color = float4(input.Color.x, input.Color.y, input.Color.z, 1.0f);
	color.w *= circleAlpha;

	PSOutput output = (PSOutput)0;
	output.Color = color;
	return output;
}