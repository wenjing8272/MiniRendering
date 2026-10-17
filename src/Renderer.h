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
		void launchRenderer(int width, int height);
	private:
		Context m_context;
		Window m_window;
	};
}