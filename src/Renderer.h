#pragma once
#include"core/Context.h"
#include"core/WindowSystem.h"
#include"render/SwapChain.h"
#include"render/MR_Command.h"
#include"render/MainDraw.h"
#include"render/PipelineLib.h"

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
		PipelineLib m_pipeline;
		MainDraw m_draw;
	};
}