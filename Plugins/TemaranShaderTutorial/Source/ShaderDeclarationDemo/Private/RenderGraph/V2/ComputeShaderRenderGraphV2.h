// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RenderGraph.h"

struct FShaderUsageExampleParameters;

class FComputeShaderRenderGraphV2
{
public:
	static void RunComputeShader_RenderGraph(FRDGBuilder& GraphBuilder, const FShaderUsageExampleParameters& DrawParameters, FRDGTextureRef ComputeShaderOutput);
};
