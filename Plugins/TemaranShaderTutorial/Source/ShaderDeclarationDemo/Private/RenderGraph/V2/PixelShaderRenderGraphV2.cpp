// Fill out your copyright notice in the Description page of Project Settings.

#include "PixelShaderRenderGraphV2.h"
#include "GlobalShader.h"
#include "ShaderDeclarationDemoModule.h"
#include "PixelShaderUtils.h"
#include "RenderTargetPool.h"


class FRenderGraphPSV2 : public FGlobalShader
{
public:
	DECLARE_GLOBAL_SHADER(FRenderGraphPSV2);
	SHADER_USE_PARAMETER_STRUCT(FRenderGraphPSV2, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER_RDG_TEXTURE(Texture2D<float4>, ComputeShaderOutput)
		RENDER_TARGET_BINDING_SLOTS()
	END_SHADER_PARAMETER_STRUCT()

public:
	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
	}
};

IMPLEMENT_GLOBAL_SHADER(FRenderGraphPSV2, "/TutorialShaders/Private/PixelShader2.usf", "PassthroughPixelShader", SF_Pixel);

void FPixelShaderRenderGraphV2::DrawToRenderTarget_RenderGraph(FRDGBuilder& GraphBuilder, const FShaderUsageExampleParameters& DrawParameters, FRDGTextureRef ComputeShaderOutput, TRefCountPtr<IPooledRenderTarget> RenderTarget)
{
	auto ShaderMap = GetGlobalShaderMap(GMaxRHIFeatureLevel);
	TShaderMapRef<FRenderGraphPSV2> PixelShader(ShaderMap);

	FRenderGraphPSV2::FParameters* PassParameters = GraphBuilder.AllocParameters<FRenderGraphPSV2::FParameters>();
	PassParameters->ComputeShaderOutput = ComputeShaderOutput;

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
		RDG_EVENT_NAME("Pixel Shader V2 (Render Graph)"),
		*PixelShader,
		PassParameters,
		FIntRect(0, 0, DrawParameters.GetRenderTargetSize().X, DrawParameters.GetRenderTargetSize().Y));
}
