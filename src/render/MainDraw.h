#pragma once
#include<vulkan/vulkan_raii.hpp>
#include<vector>

namespace MiniRendering {

	class PipelineLib;

	class MainDraw {
	public:
		MainDraw() = default;
		~MainDraw();
		MainDraw(const MainDraw& other) = delete;
		MainDraw& operator=(const MainDraw& other) = delete;
		void init(const std::vector<vk::raii::CommandBuffer>& commandBuffers, const PipelineLib& pipeline);
		void draw();
	private:
		const std::vector<vk::raii::CommandBuffer>* m_commandBuffers = nullptr;
		const PipelineLib* m_pipeline = nullptr;
	};
}