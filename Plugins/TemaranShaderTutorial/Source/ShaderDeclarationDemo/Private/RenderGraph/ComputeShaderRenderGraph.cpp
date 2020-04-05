// Fill out your copyright notice in the Description page of Project Settings.

#include "ComputeShaderRenderGraph.h"
#include "ShaderDeclarationDemoModule.h"

#define NUM_THREADS_PER_GROUP_DIMENSION 32

class FComputeShaderRenderGraphCS : public FGlobalShader
{
public:
	DECLARE_GLOBAL_SHADER(FComputeShaderRenderGraphCS);
	SHADER_USE_PARAMETER_STRUCT(FComputeShaderRenderGraphCS, FGlobalShader);

	BEGIN_SHADER_PARAMETER_STRUCT(FParameters, )
		SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<uint>, OutputTexture)
		SHADER_PARAMETER(FVector2D, TextureSize) // Metal doesn't support GetDimensions(), so we send in this data via our parameters.
		SHADER_PARAMETER(float, SimulationState)
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
IMPLEMENT_GLOBAL_SHADER(FComputeShaderRenderGraphCS, "/TutorialShaders/Private/ComputeShader.usf", "MainComputeShader", SF_Compute);

void FComputeShaderRenderGraph::RunComputeShader_RenderGraph(FRDGBuilder& GraphBuilder, const FShaderUsageExampleParameters& DrawParameters, FRDGTextureRef ComputeShaderOutput)
{
	FComputeShaderRenderGraphCS::FParameters* PassParameters = GraphBuilder.AllocParameters<FComputeShaderRenderGraphCS::FParameters>();
	PassParameters->OutputTexture = GraphBuilder.CreateUAV(ComputeShaderOutput);;
	PassParameters->TextureSize = FVector2D(DrawParameters.GetRenderTargetSize().X, DrawParameters.GetRenderTargetSize().Y);
	PassParameters->SimulationState = DrawParameters.SimulationState;

	TShaderMapRef<FComputeShaderRenderGraphCS> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));

	FComputeShaderUtils::AddPass(
		GraphBuilder,
		RDG_EVENT_NAME("Computer Shader (Render Graph)"),
		*ComputeShader,
		PassParameters,
		FComputeShaderUtils::GetGroupCount(DrawParameters.GetRenderTargetSize(), NUM_THREADS_PER_GROUP_DIMENSION)
	);
}
