// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RenderGraph.h"

struct FShaderUsageExampleParameters;

class FComputeShaderRenderGraph
{
public:
	static void RunComputeShader_RenderGraph(FRDGBuilder& GraphBuilder, const FShaderUsageExampleParameters& DrawParameters, FRDGTextureRef ComputeShaderOutput);
};
