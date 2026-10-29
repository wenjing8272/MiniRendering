#pragma once
#include<vulkan/vulkan_raii.hpp>
#include"core/DeviceContext.h"
#include<cstdint>
#include<array>
#include<filesystem>

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
		void createPipeline(const DeviceContext& deviceContext, vk::Format format, const std::filesystem::path& shadersDirectory);
	private:
		vk::raii::ShaderModule loadShader(const vk::raii::Device& device, const std::filesystem::path& shaderFile);
		void createGraphicsPipeline(const DeviceContext& deviceContext, vk::Format format, const std::filesystem::path& shadersDirectory);
		void createComputePipeline(const DeviceContext& deviceContext, const std::filesystem::path& shadersDirectory);
		std::array<PerPipelineSource, 2> m_pipelineLib;
	};
}