#pragma once
#include"core/Context.h"
#include"core/WindowSystem.h"

namespace MiniRendering {
	class Renderer
	{
	public:
		Renderer() = default;
		~Renderer();
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept;
		Renderer& operator=(Renderer&&) noexcept;
		void launchRenderer(int width, int height);
	private:
		Window m_window;
		Context m_context;
	};
}