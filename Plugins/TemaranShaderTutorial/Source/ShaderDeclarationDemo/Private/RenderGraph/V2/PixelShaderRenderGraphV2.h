// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RenderGraph.h"

struct FShaderUsageExampleParameters;

class FPixelShaderRenderGraphV2
{
public:
	static void DrawToRenderTarget_RenderGraph(FRDGBuilder& GraphBuilder, const FShaderUsageExampleParameters& DrawParameters, FRDGTextureRef ComputeShaderOutput, TRefCountPtr<IPooledRenderTarget> RenderTarget);
};
