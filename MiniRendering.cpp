// MiniRendering.cpp: 定义应用程序的入口点。
//
#include"Renderer.h"
#include"utils/Logger.h"

//项目在CMake配置为动态链接需要初始化函数表
//初始化全局函数配置基础函数指针
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

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
