#include"MainDraw.h"
#include"PipelineLib.h"
#include<Logger.h>

namespace MiniRendering {
	MainDraw::~MainDraw()
	{
		MR_LOG_INFO("destory main draw");
	}
	void MainDraw::init(const std::vector<vk::raii::CommandBuffer>& commandBuffers, const PipelineLib& pipeline)
	{
		m_commandBuffers = &commandBuffers;
		m_pipeline = &pipeline;
	}
	void MainDraw::draw()
	{
		MR_LOG_INFO("draw");
	}
}