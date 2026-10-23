#pragma once
#include<vulkan/vulkan_raii.hpp>
#include<cstdint>
#include<array>

namespace MiniRendering {
	enum class PipelineType: std::uint8_t
	{
		graphics,
		compute
	};
	struct PerPipelineSource{
		vk::raii::PipelineLayout pLayout{ nullptr };
		vk::raii::Pipeline pPipeline{ nullptr };
	};
	class PipelineLib {
	public:
		PipelineLib() = default;
		~PipelineLib();
		PipelineLib(const PipelineLib& other) = delete;
		PipelineLib& operator=(const PipelineLib& other) = delete;
		void createPipeline(const vk::raii::Device& device);
	private:
		std::array<PerPipelineSource, 2> m_pipelineLib;
	};
}