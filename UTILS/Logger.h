#pragma once

#include <spdlog/spdlog.h>

#include <cstdint>
#include <memory>
#include <source_location>
#include <stdexcept>
#include <string>

namespace MiniRendering
{
    class LogSystem final
    {
    public:
        enum class LogLevel : uint8_t
        {
            debug,
            info,
            warn,
            error,
            fatal
        };

    public:
        LogSystem();
        ~LogSystem();

        static void log(LogLevel level, const std::string& msg,
                        const std::source_location loc = std::source_location::current())
        {
            const spdlog::source_loc sloc{ loc.file_name(), static_cast<int>(loc.line()), loc.function_name() };
            switch (level)
            {
            case LogLevel::debug:
                m_logger->log(sloc, spdlog::level::debug, msg);
                break;
            case LogLevel::info:
                m_logger->log(sloc, spdlog::level::info, msg);
                break;
            case LogLevel::warn:
                m_logger->log(sloc, spdlog::level::warn, msg);
                break;
            case LogLevel::error:
                m_logger->log(sloc, spdlog::level::err, msg);
                break;
            case LogLevel::fatal:
                m_logger->log(sloc, spdlog::level::critical, msg);
                throw std::runtime_error(msg);
                break;
            default:
                break;
            }
        }

    private:
        static std::shared_ptr<spdlog::logger> m_logger;
    };

} // namespace MiniRendering
