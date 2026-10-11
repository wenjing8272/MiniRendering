// MiniRendering.cpp: 定义应用程序的入口点。
//

#include"src/Renderer.h"
#include"UTILS/Logger.h"

int main()
{
    // 格式：时间 + 彩色级别 + 文件:行号 + 消息
	MiniRendering::LogSystem LogSystem;

    // 可选：写文件（崩溃了能翻日志）
    // auto fileLog = spdlog::basic_logger_mt("file", "logs/renderer.log");
    // spdlog::set_default_logger(fileLog);

    MiniRendering::LogSystem::log(MiniRendering::LogSystem::LogLevel::info, "load rendering engine");
	MiniRendering::Renderer renderer;
	renderer.launchRenderer(1920, 1080);

	return EXIT_SUCCESS;
}
