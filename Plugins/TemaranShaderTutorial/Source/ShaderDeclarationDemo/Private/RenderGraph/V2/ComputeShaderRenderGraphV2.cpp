// Fill out your copyright notice in the Description page of Project Settings.

#include "ComputeShaderRenderGraphV2.h"
#include "ShaderDeclarationDemoModule.h"
#include "Engine/Texture2D.h"


#define NUM_THREADS_PER_GROUP_DIMENSION 32

class FComputeShaderRenderGraphCSV2 : public FGlobalShader
{
public:
	DECLARE_GLOBAL_SHADER(FComputeShaderRenderGraphCSV2);
	SHADER_USE_PARAMETER_STRUCT(FComputeShaderRenderGraphCSV2, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<float4>, OutputTexture)
		SHADER_PARAMETER(FVector2D, TextureSize) // Metal doesn't support GetDimensions(), so we send in this data via our parameters.
		SHADER_PARAMETER_TEXTURE(Texture2D, InputTexture)
		SHADER_PARAMETER(FVector2D, InputTextureSize)
	END_SHADER_PARAMETER_STRUCT()

public:
	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
	{
		return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
	}

	static inline void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
	{
		FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);

		OutEnvironment.SetDefine(TEXT("THREADGROUPSIZE_X"), NUM_THREADS_PER_GROUP_DIMENSION);
		OutEnvironment.SetDefine(TEXT("THREADGROUPSIZE_Y"), NUM_THREADS_PER_GROUP_DIMENSION);
		OutEnvironment.SetDefine(TEXT("THREADGROUPSIZE_Z"), 1);
	}
};

// This will tell the engine to create the shader and where the shader entry point is.
//                            ShaderType                            ShaderPath                     Shader function name    Type
IMPLEMENT_GLOBAL_SHADER(FComputeShaderRenderGraphCSV2, "/TutorialShaders/Private/ComputeShader2.usf", "MainComputeShader", SF_Compute);

void FComputeShaderRenderGraphV2::RunComputeShader_RenderGraph(FRDGBuilder& GraphBuilder, const FShaderUsageExampleParameters& DrawParameters, FRDGTextureRef ComputeShaderOutput)
{
	FComputeShaderRenderGraphCSV2::FParameters* PassParameters = GraphBuilder.AllocParameters<FComputeShaderRenderGraphCSV2::FParameters>();
	PassParameters->OutputTexture = GraphBuilder.CreateUAV(ComputeShaderOutput);;
	PassParameters->TextureSize = FVector2D(DrawParameters.GetRenderTargetSize().X, DrawParameters.GetRenderTargetSize().Y);

	if (DrawParameters.InputTexture)
	{
		// TODO: figure out a better way to do this as this is not very safe...
		PassParameters->InputTexture = DrawParameters.InputTexture->TextureReference.TextureReferenceRHI;
		PassParameters->InputTextureSize = FVector2D(DrawParameters.InputTexture->GetSizeX(), DrawParameters.InputTexture->GetSizeY());
	}

	TShaderMapRef<FComputeShaderRenderGraphCSV2> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));

	FComputeShaderUtils::AddPass(
		GraphBuilder,
		RDG_EVENT_NAME("Computer Shader V2 (Render Graph)"),
		*ComputeShader,
		PassParameters,
		FComputeShaderUtils::GetGroupCount(DrawParameters.GetRenderTargetSize(), NUM_THREADS_PER_GROUP_DIMENSION)
	);
}
