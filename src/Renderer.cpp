#include"Renderer.h"
#include<Logger.h>
#include<QueueIndex.h>

namespace MiniRendering {
	Renderer::~Renderer()
	{
		MiniRendering::LogSystem::log(MiniRendering::LogSystem::LogLevel::info, "destroy renderer");
	}
	Renderer::Renderer(Renderer&& other) noexcept
		: m_context(std::move(other.m_context)), m_window(std::move(other.m_window))
	{
	}
	Renderer& Renderer::operator=(Renderer&& other) noexcept
	{
		if (this != &other)
		{
			m_context = std::move(other.m_context);
			m_window = std::move(other.m_window);
		}
		return *this;
	}
	void Renderer::launchRenderer(int width, int height)
	{
		m_window.windowInit(width, height);
		auto requiredExtensions = m_window.getGLFWExtension();
		m_context.initContext(requiredExtensions);
		m_window.createSurface(m_context.getInstance());
		MiniRendering::QueueIndex queueIndex = MiniRendering::QueueIndex::find(m_context.getPhysicalDevice(), m_window.getSurface());
		m_context.createDevice(queueIndex);
		while (!glfwWindowShouldClose(m_window.getWindow()))
		{
			glfwPollEvents();
		}
	}
} // namespace MiniRendering