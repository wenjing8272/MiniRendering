# CMakeList.txt: MiniRendering 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.10)

# 如果支持，请为 MSVC 编译器启用热重载。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()
project ("MiniRendering")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)
add_subdirectory(third_party/glm)
add_library(commonCompileConfigure INTERFACE)

target_compile_definitions(commonCompileConfigure INTERFACE
    VULKAN_HPP_NO_STRUCT_CONSTRUCTORS=1             # 禁用 vulkan-hpp 聚合初始化
    VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1        # vulkan-hpp 用动态加载
    TINYGLTF_NO_STB_IMAGE                             
    TINYGLTF_NO_STB_IMAGE_WRITE             #  禁用tinygltf内置的 stb_Image
    $<$<CONFIG:Debug>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG>
    $<$<CONFIG:Release>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_WARN>      # 编译期级别：Debug 全开，Release 只留 warn/error
)
# stb
add_library(stb STATIC third_party/stb/stb_image.cpp)
target_include_directories(stb PUBLIC third_party/stb) 
# tinyglTFLoader
add_library(tinygltf STATIC third_party/tinyglTFloader/tiny_gltf.cpp)
target_include_directories(tinygltf PUBLIC third_party/tinyglTFloader)
target_link_libraries(tinygltf PUBLIC commonCompileConfigure stb)

# VAM
add_library(VMA STATIC third_party/VMA/vk_mem_alloc.cpp)
target_include_directories(VMA PUBLIC third_party/VMA)
target_link_libraries(VMA PUBLIC commonCompileConfigure Vulkan::Vulkan)
# GLFW（窗口 + surface）
set(GLFW_LIB "${CMAKE_CURRENT_SOURCE_DIR}/third_party/GLFW/glfw-3.5.1.bin.WIN64/lib-vc2022/glfw3.lib")
# core源代码添加到此项目的可执行文件。
add_library(core STATIC 
    src/core/Context.cpp
    src/core/WindowSystem.cpp 
    src/core/Context.cpp
    src/core/QueueIndex.h
    src/core/Debug.cpp
    src/core/DeviceContext.h
)
target_include_directories(core PUBLIC src/core)
target_link_libraries(core PUBLIC commonCompileConfigure Vulkan::Vulkan ${GLFW_LIB} VMA)
target_include_directories(core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/third_party/GLFW/glfw-3.5.1.bin.WIN64/include")
# resources源代码
add_library(resources STATIC
    src/resources/Buffer.cpp
)
target_include_directories(resources PUBLIC src/resources)
target_link_libraries(resources PUBLIC core glm::glm stb)
target_include_directories(resources PUBLIC third_party/glm)

# scene源码
add_library(scene STATIC 
    src/scene/ModelLoader.cpp
)
target_include_directories(scene PUBLIC src/scene)
target_link_libraries(scene PUBLIC resources tinygltf)

# render源码
add_library(render STATIC
    src/render/SwapChain.cpp
    src/render/MR_Command.cpp
)
target_include_directories(render PUBLIC src/render)
target_link_libraries(render PUBLIC resources)

# 拉取 spdlog
include(FetchContent)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.15.0                       # 用最新稳定 tag（GitHub 上查一下）
)
FetchContent_MakeAvailable(spdlog)
# utils：通用工具（日志封装）
add_library(utils STATIC
    UTILS/Logger.cpp
)
target_include_directories(utils PUBLIC UTILS)
target_link_libraries(utils PUBLIC spdlog::spdlog)
target_link_libraries(commonCompileConfigure INTERFACE utils)

add_executable(MiniRendering
    MiniRendering.cpp
    src/Renderer.cpp
)
target_link_libraries(MiniRendering PRIVATE
    ${GLFW_LIB}
)
target_link_libraries(MiniRendering PRIVATE render scene utils)
# 运行时需要 glfw3.dll，拷贝到输出目录
add_custom_command(TARGET MiniRendering POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/third_party/GLFW/glfw-3.5.1.bin.WIN64/lib-vc2022/glfw3.dll"
        $<TARGET_FILE_DIR:MiniRendering>
)
if(MSVC)
    target_compile_options(MiniRendering PRIVATE /utf-8)
endif()
# TODO: 如有需要，请添加测试并安装目标。
