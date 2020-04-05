// Fill out your copyright notice in the Description page of Project Settings.

#include "PixelShaderRenderGraph.h"
#include "GlobalShader.h"
#include "ShaderDeclarationDemoModule.h"
#include "PixelShaderUtils.h"
#include "RenderTargetPool.h"


class FRenderGraphPS : public FGlobalShader
{
public:
	DECLARE_GLOBAL_SHADER(FRenderGraphPS);
	SHADER_USE_PARAMETER_STRUCT(FRenderGraphPS, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER_RDG_TEXTURE(Texture2D<uint>, ComputeShaderOutput)
		SHADER_PARAMETER(FVector4, StartColor)
		SHADER_PARAMETER(FVector4, EndColor)
		SHADER_PARAMETER(FVector2D, TextureSize) // Metal doesn't support GetDimensions(), so we send in this data via our parameters.
		SHADER_PARAMETER(float, BlendFactor)
		RENDER_TARGET_BINDING_SLOTS()
	END_SHADER_PARAMETER_STRUCT()

public:
	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
	}
};

IMPLEMENT_GLOBAL_SHADER(FRenderGraphPS, "/TutorialShaders/Private/PixelShader.usf", "MainPixelShaderScreenPass", SF_Pixel);

void FPixelShaderRenderGraph::DrawToRenderTarget_RenderGraph(FRDGBuilder& GraphBuilder, const FShaderUsageExampleParameters& DrawParameters, FRDGTextureRef ComputeShaderOutput, TRefCountPtr<IPooledRenderTarget> RenderTarget)
{
	auto ShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);
	TShaderMapRef<FRenderGraphPS> PixelShader(ShaderMap);

	FRenderGraphPS::FParameters* PassParameters = GraphBuilder.AllocParameters<FRenderGraphPS::FParameters>();
	PassParameters->ComputeShaderOutput = ComputeShaderOutput;
	PassParameters->StartColor = FVector4(DrawParameters.StartColor.R, DrawParameters.StartColor.G, DrawParameters.StartColor.B, DrawParameters.StartColor.A) / 255.0f;
	PassParameters->EndColor = FVector4(DrawParameters.EndColor.R, DrawParameters.EndColor.G, DrawParameters.EndColor.B, DrawParameters.EndColor.A) / 255.0f;
	PassParameters->TextureSize = FVector2D(DrawParameters.GetRenderTargetSize().X, DrawParameters.GetRenderTargetSize().Y);
	PassParameters->BlendFactor = DrawParameters.ComputeShaderBlend;

	FRDGTextureRef RDGRenderTarget;
	if (!RenderTarget)
	{
		FRDGTextureDesc Desc = FRDGTextureDesc::Create2DDesc(
			DrawParameters.GetRenderTargetSize(),
			PF_FloatRGBA,
			FClearValueBinding(FLinearColor::Red),
			TexCreate_None,
			TexCreate_RenderTargetable | TexCreate_UAV | TexCreate_ShaderResource,
			false);

		RDGRenderTarget = GraphBuilder.CreateTexture(Desc, TEXT("PSOutput"));
	}
	else
	{
		RDGRenderTarget = GraphBuilder.RegisterExternalTexture(RenderTarget, TEXT("PSOutput"));		
	}

	PassParameters->RenderTargets[0] = FRenderTargetBinding(RDGRenderTarget, ERenderTargetLoadAction::EClear);

	FPixelShaderUtils::AddFullscreenPass(
		GraphBuilder,
		ShaderMap,
		RDG_EVENT_NAME("Pixel Shader Example"),
		*PixelShader,
		PassParameters,
		FIntRect(0, 0, DrawParameters.GetRenderTargetSize().X, DrawParameters.GetRenderTargetSize().Y));
}
