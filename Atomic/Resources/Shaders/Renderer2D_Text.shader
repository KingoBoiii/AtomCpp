struct VSInput
{
	float3 Position : POSITION;
	float4 Color : COLOR;
	float2 TexCoord : TEXCOORD;
};

struct VSOutput
{
	float4 PositionCS : SV_POSITION;
	float4 Position : POSITION;
	float4 Color : COLOR;
	float2 TexCoord : TEXCOORD;
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
	output.TexCoord = input.TexCoord;
	return output;
}

Texture2D FontAtlasTexture : register(t0);
SamplerState FontAtlasSampler : register(s0);

struct PSOutput
{
	float4 Color : SV_TARGET;
};

float median(float r, float g, float b)
{
	return max(min(r, g), min(max(r, g), b));
}

float screenPxRange(float2 texCoord)
{
	float pxRange = 2.0f;

	float width, height, levels;
	FontAtlasTexture.GetDimensions(0, width, height, levels);

	float2 unitRange = float2(pxRange, pxRange) / float2(width, height);
	float2 screenTexSize = float2(1.0f, 1.0f) / fwidth(texCoord);
	return max(0.5 * dot(unitRange, screenTexSize), 1.0);
}

PSOutput PSMain(VSOutput input)
{
	float4 msdf = FontAtlasTexture.Sample(FontAtlasSampler, input.TexCoord);
	float3 msd = msdf.rgb;

	float sd = median(msd.r, msd.g, msd.b);

	float screenPxDistance = screenPxRange(input.TexCoord) * (sd - 0.5f);

	float opacity = clamp(screenPxDistance + 0.5f, 0.0f, 1.0f);
	if(opacity == 0.0f)
	{
		discard;
	}

	float4 bgColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
		
	float4 color = lerp(bgColor, input.Color, opacity);
	if(color.a == 0.0f)
	{
		discard;
	}

	PSOutput output = (PSOutput)0;
	output.Color = color;
	return output;
}