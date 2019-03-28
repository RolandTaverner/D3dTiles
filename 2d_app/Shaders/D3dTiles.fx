//--------------------------------------------------------------------------------------
// File: Tutorial04.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register(b0)
{
  matrix World;
  matrix View;
  matrix Projection;
}

Texture2D tex2D;
SamplerState linearSampler
{
  Filter = MIN_MAG_MIP_LINEAR;
  AddressU = Wrap;
  AddressV = Wrap;
};

struct VS_INPUT
{
  float4 position : POSITION;
  float2 tex : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
  float4 position : SV_POSITION;
  float2 tex : TEXCOORD0;
};

//BlendState AlphaBlending
//{
//    BlendEnable[0] = TRUE;
//    SrcBlend = SRC_ALPHA;
//    DestBlend = INV_SRC_ALPHA;
//    BlendOp = ADD;
//    SrcBlendAlpha = ONE;
//    DestBlendAlpha = ZERO;
//    BlendOpAlpha = ADD;
//    RenderTargetWriteMask[0] = 0x0F;
//};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
  VS_OUTPUT output;

  input.position.w = 1.0f;
  output.position = mul(input.position, World);
  output.position = mul(output.position, View);
  output.position = mul(output.position, Projection);
  output.tex = input.tex;
  return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
  float4 textureColor;

  // Sample the pixel color from the texture using the sampler at this texture coordinate location.
  textureColor = tex2D.Sample(linearSampler, input.tex);
  return textureColor;
}

//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
//technique10 render {
//  pass P0 {
//    SetVertexShader(CompileShader(vs_4_0, VS()));
//    SetGeometryShader(NULL);
//    SetPixelShader(CompileShader(ps_4_0, PS()));
//
//    SetBlendState(AlphaBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
//  }
//}