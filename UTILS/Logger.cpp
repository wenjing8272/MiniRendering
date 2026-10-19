#include "Logger.h"

#include <spdlog/async.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#ifdef _WIN32
#include <spdlog/sinks/msvc_sink.h>
#endif

#include <cstdio>
#include <filesystem>
#include <vector>

namespace MiniRendering
{
    std::shared_ptr<spdlog::logger> LogSystem::m_logger = nullptr;
    std::shared_ptr<spdlog::logger> LogSystem::m_fatalLogger = nullptr;

    spdlog::level::level_enum LogSystem::toSpdlog(LogLevel level) noexcept
    {
        switch (level)
        {
        case LogLevel::debug: return spdlog::level::debug;
        case LogLevel::info:  return spdlog::level::info;
        case LogLevel::warn:  return spdlog::level::warn;
        case LogLevel::error: return spdlog::level::err;
        case LogLevel::fatal: return spdlog::level::critical;
        }
        return spdlog::level::off;
    }

    void LogSystem::init(const LogOptions& options)
    {
        if (m_logger) return;

        std::vector<spdlog::sink_ptr> sinks;

        auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console->set_level(spdlog::level::trace);
        console->set_pattern(options.pattern);
        sinks.push_back(console);

#ifdef _WIN32
#ifndef NDEBUG
        auto debugger = std::make_shared<spdlog::sinks::msvc_sink_mt>();
        debugger->set_level(spdlog::level::trace);
        debugger->set_pattern(options.pattern);
        sinks.push_back(debugger);
#endif
#endif

        if (!options.filePath.empty())
        {
            const std::filesystem::path path(options.filePath);
            if (path.has_parent_path())
                std::filesystem::create_directories(path.parent_path());

            auto file = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
                options.filePath, 8 * 1024 * 1024, 3);
            file->set_level(spdlog::level::trace);
            file->set_pattern(options.pattern);
            sinks.push_back(file);
        }

        if (options.async)
        {
            spdlog::init_thread_pool(options.queueSize, 1);
            m_logger = std::make_shared<spdlog::async_logger>(
                "MiniRendering", sinks.begin(), sinks.end(),
                spdlog::thread_pool(), spdlog::async_overflow_policy::block);
        }
        else
        {
            m_logger = std::make_shared<spdlog::logger>(
                "MiniRendering", sinks.begin(), sinks.end());
        }
        m_logger->set_level(options.level);
        m_logger->flush_on(spdlog::level::err);

        // 同步 fatal logger，和主 logger 共用 sink
        m_fatalLogger = std::make_shared<spdlog::logger>(
            "MiniRendering_fatal", sinks.begin(), sinks.end());
        m_fatalLogger->set_level(spdlog::level::trace);

        spdlog::register_logger(m_logger);
        spdlog::set_default_logger(m_logger);   // 让 SPDLOG_* 宏也走这里
        spdlog::set_error_handler([](const std::string& msg) {
            std::fprintf(stderr, "spdlog error: %s\n", msg.c_str());
            });
    }

    void LogSystem::shutdown()
    {
        if (m_logger)
        {
            m_logger->flush();
            spdlog::drop(m_logger->name());
        }
        m_logger.reset();
        m_fatalLogger.reset();
    }

    void LogSystem::setLevel(LogLevel level) noexcept
    {
        if (m_logger) m_logger->set_level(toSpdlog(level));
    }

    bool LogSystem::shouldLog(LogLevel level) noexcept
    {
        return m_logger && m_logger->should_log(toSpdlog(level));
    }
}
