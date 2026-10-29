#pragma once
#include"render/sync/FrameResource.h"
#include<vulkan/vulkan_raii.hpp>
#include<vector>

namespace MiniRendering {

	class PipelineLib;

	class SwapChain;

	class MainDraw {
	public:
		MainDraw() = default;
		~MainDraw();
		MainDraw(const MainDraw& other) = delete;
		MainDraw& operator=(const MainDraw& other) = delete;
		void init(const PipelineLib& pipelineLib);
		void recordCommand(const FrameResource& frame,const SwapChain& swapChain);
	private:
		const PipelineLib* m_pipelineLib = nullptr;
	};
}