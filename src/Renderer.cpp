#include"Renderer.h"
#include<Logger.h>
#include<QueueIndex.h>

namespace MiniRendering {
	Renderer::~Renderer()
	{
		MiniRendering::LogSystem::log(MiniRendering::LogSystem::LogLevel::info, "destroy renderer");
	}
	void Renderer::launchRenderer(int width, int height)
	{
		m_window.windowInit(width, height);
		auto requiredExtensions = m_window.getGLFWExtension();
		m_context.initInstance(requiredExtensions);
		m_window.createSurface(m_context.getInstance());
		m_context.pickPhysicalDevice(m_window.getSurface());
		MiniRendering::QueueIndex queueIndex = MiniRendering::QueueIndex::find(m_context.getPhysicalDevice(), m_window.getSurface());
		m_context.createDevice(queueIndex);
		while (!glfwWindowShouldClose(m_window.getWindow()))
		{
			glfwPollEvents();
		}
	}
} // namespace MiniRendering