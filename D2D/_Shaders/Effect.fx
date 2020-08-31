cbuffer CB_PerFrame
{
	matrix View;
	matrix Projection;
}

matrix World;

//-----------------------------------------------------------------------------
//Types
//-----------------------------------------------------------------------------
struct VertexInput
{
	float4 Position : POSITION0;
	float2 Uv : UV0;
};

struct VertexOutput
{
	float4 Position : SV_POSITION0;
	float2 Uv : UV0;
};

//-----------------------------------------------------------------------------
//Vertex Shader
//-----------------------------------------------------------------------------
VertexOutput VS(VertexInput input)
{
	VertexOutput output;

	output.Position = mul(input.Position, World);
	output.Position = mul(output.Position, View);
	output.Position = mul(output.Position, Projection);

	output.Uv = input.Uv;

	return output;
}

//-----------------------------------------------------------------------------
//Pixel Shader
//-----------------------------------------------------------------------------
Texture2D DiffuseMap;
SamplerState Sampler;

float4 PS(VertexOutput input) : SV_TARGET0
{
	return DiffuseMap.Sample(Sampler, input.Uv);
}

//-----------------------------------------------------------------------------
//States
//-----------------------------------------------------------------------------
BlendState AlphaBlend
{
	BlendEnable[0] = true;
	DestBlend[0] = INV_SRC_ALPHA;
	SrcBlend[0] = SRC_ALPHA;
	BlendOp[0] = Add;

	SrcBlendAlpha[0] = One;
	DestBlendAlpha[0] = One;
	RenderTargetWriteMask[0] = 0x0F;
};

RasterizerState Cull
{
	//Clip �� Cull�� ����
	//Clip -> 2D
	//Cull -> 3D
	Cullmode = None;
	DepthClipEnable = false;
};

//-----------------------------------------------------------------------------
//Pipe Line
//-----------------------------------------------------------------------------
technique11 T0
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));

		SetRasterizerState(Cull);
		SetBlendState(AlphaBlend, float4(0, 0, 0, 0), 0xFF);
	}

}