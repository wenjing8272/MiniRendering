#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>

#include <cstdint>
#include <memory>
#include <source_location>
#include <stdexcept>
#include <string>
#include <utility>

namespace MiniRendering
{
    enum class LogLevel : std::uint8_t { debug, info, warn, error, fatal };

    struct LogOptions
    {
        bool async = true;
        spdlog::level::level_enum level = spdlog::level::debug;
        std::size_t queueSize = 8192;            //
        std::string filePath;                
        std::string pattern = "[%Y-%m-%d %H:%M:%S.%e] [%t] [%^%l%$] [%s:%#] %v";
    };

    class LogSystem final
    {
    public:
        LogSystem() = delete;

        static void init(const LogOptions& options = {});
        static void shutdown();
        static void setLevel(LogLevel level) noexcept;
        static bool shouldLog(LogLevel level) noexcept;

        template <class... Args>
        static void logAt(LogLevel level, const std::source_location& loc,
            spdlog::format_string_t<Args...> fmt, Args&&... args)
        {
            const spdlog::source_loc sloc{
                loc.file_name(), static_cast<int>(loc.line()), loc.function_name() };

            // fatal 走同步 logger，保证消息在 throw 前真正写进 sink
            if (level == LogLevel::fatal)
            {
                const std::string message =
                    spdlog::fmt_lib::format(fmt, std::forward<Args>(args)...);

                if (m_fatalLogger)
                {
                    m_fatalLogger->log(sloc, spdlog::level::critical, "{}", message);
                    m_fatalLogger->flush();
                }
                else if (m_logger)
                {
                    m_logger->log(sloc, spdlog::level::critical, "{}", message);
                    m_logger->flush();
                }
                throw std::runtime_error(message);
            }

            if (!m_logger || !m_logger->should_log(toSpdlog(level)))
                return;

            m_logger->log(sloc, toSpdlog(level), fmt, std::forward<Args>(args)...);
        }

    private:
        static spdlog::level::level_enum toSpdlog(LogLevel level) noexcept;

        static std::shared_ptr<spdlog::logger> m_logger;       // 主 logger，可异步
        static std::shared_ptr<spdlog::logger> m_fatalLogger;  // 同步，仅 fatal 用
    };

} // namespace MiniRendering

#define MR_LOG(level, ...) \
      ::MiniRendering::LogSystem::logAt(level, std::source_location::current(), __VA_ARGS__)

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_DEBUG
#define MR_LOG_DEBUG(...) MR_LOG(::MiniRendering::LogLevel::debug, __VA_ARGS__)
#else
#define MR_LOG_DEBUG(...) (void)0
#endif

#if SPDLOG_ACTIVE_LEVEL <= SPDLOG_LEVEL_INFO
#define MR_LOG_INFO(...) MR_LOG(::MiniRendering::LogLevel::info, __VA_ARGS__)
#else
#define MR_LOG_INFO(...) (void)0
#endif

#define MR_LOG_WARN(...)  MR_LOG(::MiniRendering::LogLevel::warn, __VA_ARGS__)
#define MR_LOG_ERROR(...) MR_LOG(::MiniRendering::LogLevel::error, __VA_ARGS__)
#define MR_LOG_FATAL(...) MR_LOG(::MiniRendering::LogLevel::fatal, __VA_ARGS__)