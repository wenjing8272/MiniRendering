#include"render/passes/MainDraw.h"
#include"render/pipeline/PipelineLib.h"
#include"render/swapchain/SwapChain.h"
#include"render/Barrier.h"
#include"utils/Logger.h"

namespace MiniRendering {
	MainDraw::~MainDraw()
	{
		MR_LOG_INFO("destory main draw");
	}
	void MainDraw::init(const PipelineLib& pipelineLib)
	{
		m_pipelineLib = &pipelineLib;
	}
	void MainDraw::recordCommand(const FrameResource& frame,const SwapChain& swapChain)
	{
		auto& commandBuffer = frame.command;
		commandBuffer.begin(vk::CommandBufferBeginInfo{});
		transitionImageLayout(
			commandBuffer,
			swapChain.getSwapChainImages()[frame.imageIndex],
			vk::ImageLayout::eUndefined,
			vk::ImageLayout::eColorAttachmentOptimal,
			vk::AccessFlagBits2::eNone,
			vk::AccessFlagBits2::eColorAttachmentWrite,
			vk::PipelineStageFlagBits2::eNone,
			vk::PipelineStageFlagBits2::eColorAttachmentOutput,
			vk::ImageAspectFlagBits::eColor
		);
		vk::ClearValue clearColor = vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f);
		vk::RenderingAttachmentInfo swapChainAttachmentInfo{
			.imageView = swapChain.getSwapChainImageViews()[frame.imageIndex],
			.imageLayout = vk::ImageLayout::eColorAttachmentOptimal,
			.loadOp = vk::AttachmentLoadOp::eClear,
			.storeOp = vk::AttachmentStoreOp::eStore,
			.clearValue = clearColor
		};
		vk::RenderingInfo renderingInfo{
			.renderArea = {.offset = {0,0},.extent = swapChain.getSwapChainExtent()},
			.layerCount = 1,
			.colorAttachmentCount = 1,
			.pColorAttachments = &swapChainAttachmentInfo,
		};
		commandBuffer.beginRendering(renderingInfo);

		commandBuffer.endRendering();
		transitionImageLayout(
			commandBuffer,
			swapChain.getSwapChainImages()[frame.imageIndex],
			vk::ImageLayout::eColorAttachmentOptimal,
			vk::ImageLayout::ePresentSrcKHR,
			vk::AccessFlagBits2::eColorAttachmentWrite,
			{},
			vk::PipelineStageFlagBits2::eColorAttachmentOutput,
			vk::PipelineStageFlagBits2::eBottomOfPipe,
			vk::ImageAspectFlagBits::eColor
		);
		commandBuffer.end();
	}
}