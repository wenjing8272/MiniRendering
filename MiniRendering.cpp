// MiniRendering.cpp: 定义应用程序的入口点。
//
#include"src/Renderer.h"
#include"UTILS/Logger.h"

struct LogGuard
{
    explicit LogGuard(const MiniRendering::LogOptions& options)
    {
        MiniRendering::LogSystem::init(options);
    }
    ~LogGuard()
    {
        MiniRendering::LogSystem::shutdown();
    }
};

int main()
{
    MiniRendering::LogOptions options;
    LogGuard logGuard(options);

    MR_LOG_INFO("load rendering engine");

    MiniRendering::Renderer renderer;
    renderer.launchRenderer(1920, 1080);

    return EXIT_SUCCESS;   // renderer 先析构，守卫最后析构
}
