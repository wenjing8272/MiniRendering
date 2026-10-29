#include"Renderer.h"
#include"utils/Logger.h"
#include"core/QueueIndex.h"
#include<optional>

namespace MiniRendering {
	Renderer::~Renderer()
	{
		m_context.getDevice().waitIdle();
		MR_LOG_INFO("destroy renderer");
	}
	void Renderer::launchRenderer(int width, int height)
	{
		//初始化窗口
		m_window.windowInit(width, height);                              
		auto requiredExtensions = m_window.getGLFWExtension();
		//获取需要的窗口扩展，初始化Instance
		m_context.initInstance(requiredExtensions);
		//创建surface
		m_window.createSurface(m_context.getInstance());
		//找到合适的显卡
		m_context.pickPhysicalDevice(m_window.getSurface());
		//找到合适的队列索引
		MiniRendering::QueueIndex queueIndex = MiniRendering::QueueIndex::find(m_context.getPhysicalDevice(), m_window.getSurface());
		//创建逻辑设备
		m_context.createDevice(queueIndex);
		//创建交换链
		m_swapChain.createSwapChain(m_context.deviceContext(),m_window.getSurface(),m_window.getFramebufferExtent());
		//创建渲染信号
		m_swapChain.createRenderingSemaphore(m_context.getDevice());
		m_command.createCommandPool(m_context.deviceContext(), queueIndex);
		m_command.createCommandBuffers(m_context.getDevice());
		//创建管线
		m_pipelineLib.createPipeline(m_context.deviceContext(),m_swapChain.getSwapChainSurfaceFormat().format, "shaders");
		//初始化每帧之间的同步操作
		m_frameSynchronize.init(m_context.getDevice(),m_context.getGraphicsQueue());
		//初始化绘画系统
		m_draw.init(m_pipelineLib);
		while (!m_window.windowShouldClose())
		{
			m_window.closeWindowEvent();
			m_window.pollEvent();
			if (m_swapChainDirty)
			{
				auto extent = m_window.getFramebufferExtent();
				while (extent.width == 0 || extent.height == 0)
				{
					m_window.waitEvents();
					if (m_window.windowShouldClose())return;
					extent = m_window.getFramebufferExtent();
				}
				m_context.getGraphicsQueue().waitIdle();
				m_swapChain.recreateSwapChain(m_context.deviceContext(), m_window.getSurface(), extent);
				m_swapChainDirty = false;
			}

			auto frame = m_frameSynchronize.beginFrame(m_swapChain);
			if (!frame)
			{
				m_swapChainDirty = true;
				continue;
			}

			m_command.getCommandBuffers()[frame->frameIndex].reset();
			frame->command = m_command.getCommandBuffers()[frame->frameIndex];
			m_draw.recordCommand(*frame, m_swapChain);
			if (m_frameSynchronize.endFrame(*frame, m_swapChain))
			{
				m_swapChainDirty = true;
			}
		}
	}
} // namespace MiniRendering