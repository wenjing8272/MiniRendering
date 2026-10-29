#pragma once
#include"render/RenderConfig.h"
#include"core/Context.h"
#include"core/WindowSystem.h"
#include"render/swapchain/SwapChain.h"
#include"render/command/MR_Command.h"
#include"render/passes/MainDraw.h"
#include"render/pipeline/PipelineLib.h"
#include"render/sync/FrameSynchronize.h"
#include"render/sync/FrameResource.h"
#include<array>

namespace MiniRendering {
	class Renderer
	{
	public:
		Renderer() = default;
		~Renderer();
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		void launchRenderer(int width, int height);
	private:
		Context m_context;
		Window m_window;
		SwapChain m_swapChain;
		Command m_command;
		PipelineLib m_pipelineLib;
		MainDraw m_draw;
		FrameSynchronize m_frameSynchronize;
		bool m_swapChainDirty = false;
	};
}