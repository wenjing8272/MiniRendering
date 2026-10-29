#include"render/pipeline/PipelineLib.h"
#include"utils/Logger.h"
#include<fstream>
#include<vector>

namespace MiniRendering {
	PipelineLib::~PipelineLib()
	{
		MR_LOG_INFO("destroy pipeline library");
	}
	void PipelineLib::createPipeline(const DeviceContext& device,vk::Format format ,const std::filesystem::path& shadersDirectory)
	{
		createGraphicsPipeline(device, format, shadersDirectory);
		createComputePipeline(device, shadersDirectory);
	}
	vk::raii::ShaderModule PipelineLib::loadShader(const vk::raii::Device& device, const std::filesystem::path& shaderFile)
	{
		std::ifstream file(shaderFile, std::ios::ate | std::ios::binary);
		if (!file.is_open())
		{
			MR_LOG_FATAL("cannot open shader: {}", shaderFile.string());
		}
		const auto end = file.tellg();
		const auto bytes = static_cast<size_t>(end);
		std::vector<uint32_t> code(bytes / 4);
		file.seekg(0,std::ios::beg);
		if (!file.read(reinterpret_cast<char*>(code.data()), static_cast<std::streamsize>(bytes)))
		{
			MR_LOG_FATAL("failed to read shader: {}", shaderFile.string());
		}
		vk::ShaderModuleCreateInfo shaderModuleInfo{
			.codeSize = bytes,
			.pCode = code.data()
		};
		return vk::raii::ShaderModule(device, shaderModuleInfo);
	}
	void PipelineLib::createGraphicsPipeline(const DeviceContext& deviceContext, vk::Format format, const std::filesystem::path& shadersDirectory)
	{
		
	}
	void PipelineLib::createComputePipeline(const DeviceContext& deviceContext, const std::filesystem::path& shadersDirectory)
	{

	}
}